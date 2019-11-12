#ifndef MITAMA_MAYBE_HPP
#define MITAMA_MAYBE_HPP

#include <mitama/panic.hpp>
#include <mitama/result/factory/success.hpp>
#include <mitama/result/factory/failure.hpp>
#include <mitama/result/detail/fwd.hpp>
#include <mitama/result/detail/meta.hpp>
#include <mitama/result/traits/impl_traits.hpp>
#include <mitama/maybe/fwd/maybe_fwd.hpp>
#include <mitama/maybe/factory/just_nothing.hpp>
#include <mitama/concepts/display.hpp>
#include <mitama/concepts/pointer_like.hpp>
#include <mitama/concepts/dereferencable.hpp>
#include <mitama/concepts/satisfy.hpp>
#include <mitama/cmp/ord.hpp>

#include <fmt/core.h>
#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/hana/functional/overload_linearly.hpp>

#include <functional>
#include <memory>
#include <optional>
#include <variant>
#include <type_traits>
#include <tuple>
#include <utility>
#include <string_view>
#include <cassert>
#include <concepts>

namespace mitama {

template <class T>
    requires (std::is_object_v<std::remove_cvref_t<T>>)
          && (std::negation_v<std::is_array<std::remove_cvref_t<T>>>)
class [[nodiscard("warning: unused result which must be used")]] maybe
  : public cmp::ord<maybe<T>, maybe>
  , public cmp::ord<maybe<T>, just_t>
  , public cmp::rel<maybe<T>, nothing_t>
  , public std::conditional_t<is_nothing_v<T>, detail::_empty, cmp::rel<maybe<T>, T>>
{
    std::variant<nothing_t, just_t<T>> storage_;
    template<class, class> friend class maybe_replace_injector;
    template <class> friend class maybe;

  public:
    using value_type = std::remove_reference_t<T>;
    using reference_type = std::add_lvalue_reference_t<value_type>;

    ~maybe() = default;
    constexpr maybe(): storage_(std::in_place_type<nothing_t>) {}
    constexpr maybe(maybe const&) = default;
    constexpr maybe& operator=(maybe const&) = default;
    constexpr maybe(maybe&&) = default;
    constexpr maybe& operator=(maybe&&) = default;
    constexpr maybe(nothing_t): maybe() {}

    template <pointer_like U>
    constexpr maybe(U&& u) : storage_(std::in_place_type<nothing_t>) {
        if (u) storage_.template emplace<just_t<T>>(std::in_place, *std::forward<U>(u));
    }

    template <std::constructible_from<T> U> requires (!pointer_like<U>)
    constexpr explicit(!std::convertible_to<T, U>)
    maybe(U&& u)
        noexcept(std::is_nothrow_constructible_v<T, U&&>)
        : storage_(std::in_place_type<just_t<T>>, std::in_place, std::forward<U>(u)) {}

    template <class... Args>
        requires std::constructible_from<T, Args&&...>
    constexpr explicit maybe(std::in_place_t, Args&&... args)
        : storage_(std::in_place_type<just_t<T>>, std::in_place, std::forward<Args>(args)...) {}

    template <class U, class... Args>
        requires std::constructible_from<T, std::initializer_list<U>, Args&&...>
    constexpr explicit maybe(std::in_place_t, std::initializer_list<U> il, Args&&... args)
        : storage_(std::in_place_type<just_t<T>>, std::in_place, il, std::forward<Args>(args)...) {}

    template <class... Args>
        requires std::constructible_from<T, Args&&...>
    constexpr maybe(just_t<_just_detail::forward_mode<T>, Args...>&& fwd)
        : maybe()
    {
        std::apply([&](auto&&... args){ storage_.template emplace<just_t<T>>(std::in_place, std::forward<decltype(args)>(args)...); }, std::move(fwd)());
    }

    template <class... Args>
        requires std::constructible_from<T, Args&&...>
    constexpr maybe(just_t<_just_detail::forward_mode<>, Args...>&& fwd)
        : maybe()
    {
        std::apply([&](auto&&... args){ storage_.template emplace<just_t<T>>(std::in_place, std::forward<decltype(args)>(args)...); }, std::move(fwd)());
    }

    template <class U>
        requires std::constructible_from<T, U const&>
    constexpr maybe(just_t<U> const& j)
        noexcept(std::is_nothrow_constructible_v<T, U const&>)
        : storage_(std::in_place_type<just_t<T>>, std::in_place, j.get()) {}

    template <class U>
        requires std::constructible_from<T, U&&>
    constexpr maybe(just_t<U>&& j)
        noexcept(std::is_nothrow_constructible_v<T, U&&>)
        : storage_(std::in_place_type<just_t<T>>, std::in_place, static_cast<U&&>(j.get())) {}

    constexpr explicit operator bool() const {
        return is_just();
    }

    constexpr value_type* operator->() & {
        return &(std::get<just_t<T>>(storage_).get());
    }

    constexpr value_type const* operator->() const& {
        return &(std::get<just_t<T>>(storage_).get());
    }

    constexpr bool is_just() const {
        return std::holds_alternative<just_t<T>>(storage_);
    }

    constexpr bool is_nothing() const {
        return !is_just();
    }

    constexpr value_type& unwrap() & {
        if (is_nothing())
            PANIC("called `maybe::unwrap()` on a `nothing` value");
        return std::get<just_t<T>>(storage_).get();
    }

    constexpr std::add_const_t<std::remove_reference_t<T>>&
    unwrap() const& {
        if (is_nothing())
            PANIC("called `maybe::unwrap()` on a `nothing` value");
        return std::get<just_t<T>>(storage_).get();
    }

    constexpr value_type unwrap() && {
        if (is_nothing())
            PANIC("called `maybe::unwrap()` on a `nothing` value");
        return std::move(std::get<just_t<T>>(storage_).get());
    }

    constexpr auto as_ref() & {
        return is_just()
            ? maybe<T&>(std::in_place, unwrap())
            : nothing;
    }

    constexpr auto as_ref() const& {
        return is_just()
            ? maybe<const T&>(std::in_place, unwrap())
            : nothing;
    }

    template <class... Args>
        requires std::constructible_from<T, Args&&...>
    constexpr auto& get_or_emplace(Args&&... args) & {
        return is_just()
            ? unwrap()
            : (storage_.template emplace<just_t<T>>(std::in_place, std::forward<Args>(args)...), unwrap());
    }

    template <class F, class... Args>
        requires std::invocable<F&&, Args&&...>
              && std::constructible_from<T, std::invoke_result_t<F&&, Args&&...>>
    constexpr value_type&
    get_or_emplace_with(F&& f, Args&&... args) & {
        return is_just()
            ? unwrap()
            : (storage_.template emplace<just_t<T>>(std::in_place, std::invoke(std::forward<F>(f), std::forward<Args>(args)...)), unwrap());
    }

    template <class... Args>
        requires std::copy_constructible<T>
              && std::constructible_from<T, Args&&...>
    constexpr maybe replace(Args&&... args) & {
        auto old = static_cast<maybe<T>*>(this)->as_ref().cloned();
        static_cast<maybe<T>*>(this)->storage_.template emplace<just_t<T>>(std::in_place, std::forward<Args>(args)...);
        return old;
    }

    template <class F, class... Args>
        requires std::copy_constructible<T>
              && std::invocable<F&&, Args&&...>
              && std::constructible_from<T, std::invoke_result_t<F, Args&&...>>
    constexpr maybe replace_with(F&& f, Args&&... args) & {
        auto old = static_cast<maybe<T>*>(this)->as_ref().cloned();
        static_cast<maybe<T>*>(this)->storage_.template emplace<just_t<T>>(std::invoke(std::forward<F>(f), std::forward<Args>(args)...));
        return old;
    }

    constexpr maybe<std::remove_reference_t<T>> cloned()
        requires (std::is_lvalue_reference_v<T>)
              && std::copy_constructible<T>
    {
        auto decay_copy = [](auto&& some) -> std::remove_const_t<std::remove_reference_t<T>> { return std::forward<decltype(some)>(some); };
        return static_cast<maybe<T>const*>(this)->is_just()
            ? maybe<std::remove_reference_t<T>>{just(decay_copy(static_cast<maybe<T>const*>(this)->unwrap()))}
            : nothing;
    }

    constexpr auto flatten() const&
        requires (is_maybe<std::decay_t<T>>::value)
    { return is_just() ? unwrap().as_ref().cloned() : nothing; }

    // Since C++20, enable initialize aggregates from a parenthesized list of values;
    // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0960r3.html
    /// @brief
    ///   Returns the contained value or a default.
    ///
    /// @note
    ///   Consumes the self argument then,
    ///   if just, returns the contained value,
    ///   otherwise; if nothing, returns the default value for that type.
    constexpr T unwrap_or_default() const requires (std::default_constructible<T>)
    {
        return is_just() ? unwrap() : T();
    }

    constexpr decltype(auto) unwrap_or(std::common_with<T&> auto&& def) & {
        return is_just() ? unwrap() : std::forward<decltype(def)>(def);
    }

    constexpr decltype(auto) unwrap_or(std::common_with<T const&> auto&& def) const& {
        return is_just() ? unwrap() : std::forward<decltype(def)>(def);
    }

    constexpr decltype(auto) unwrap_or(std::common_with<T&&> auto&& def) && {
        return is_just() ? std::move(unwrap()) : std::forward<decltype(def)>(def);
    }

    constexpr decltype(auto) unwrap_or_else(std::invocable auto&& f) &
        requires std::common_with<std::invoke_result_t<decltype(f)>, T&>
    { return is_just() ? unwrap() : std::invoke(std::forward<decltype(f)>(f)); }

    constexpr decltype(auto) unwrap_or_else(std::invocable auto&& f) const&
        requires std::common_with<std::invoke_result_t<decltype(f)>, T const&>
    { return is_just() ? unwrap() : std::invoke(std::forward<decltype(f)>(f)); }

    constexpr decltype(auto) unwrap_or_else(std::invocable auto&& f) &&
        requires std::common_with<std::invoke_result_t<decltype(f)>, T&&>
    { return is_just() ? std::move(unwrap()) : std::invoke(std::forward<decltype(f)>(f)); }

    constexpr auto map(std::invocable<value_type&> auto&& f) & {
        return is_just()
            ? maybe<std::invoke_result_t<decltype(f), value_type&>>{just(std::invoke(std::forward<decltype(f)>(f), unwrap()))}
            : nothing;
    }

    constexpr auto map(std::invocable<value_type const&> auto&& f) const& {
        return is_just()
            ? maybe<std::invoke_result_t<decltype(f), value_type const&>>{just(std::invoke(std::forward<decltype(f)>(f), unwrap()))}
            : nothing;
    }

    constexpr auto map(std::invocable<value_type&&> auto&& f) && {
        return is_just()
            ? maybe<std::invoke_result_t<decltype(f), value_type &&>>{just(std::invoke(std::forward<decltype(f)>(f), std::move(unwrap())))}
            : nothing;
    }

    template <std::invocable<value_type&> F, std::common_with<std::invoke_result_t<F&&, value_type&>> U>
    constexpr decltype(auto) map_or(U&& def, F&& f) & {
        return is_just()
            ? std::invoke(std::forward<F>(f), unwrap())
            : std::forward<U>(def);
    }

    template <std::invocable<value_type const&> F, std::common_with<std::invoke_result_t<F&&, value_type const&>> U>
    constexpr decltype(auto) map_or(U&& def, F&& f) const& {
        return is_just()
            ? std::invoke(std::forward<F>(f), unwrap())
            : std::forward<U>(def);
    }

    template <std::invocable<value_type&&> F, std::common_with<std::invoke_result_t<F&&, value_type&&>> U>
    constexpr decltype(auto) map_or(U&& def, F&& f) && {
        return is_just()
            ? std::invoke(std::forward<F>(f), std::move(unwrap()))
            : std::forward<U>(def);
    }

    constexpr decltype(auto) map_or_else(std::invocable auto&& def, std::invocable<value_type&> auto&& f) & {
        return is_just()
            ? std::invoke(std::forward<decltype(f)>(f), unwrap())
            : std::invoke(std::forward<decltype(def)>(def));
    }

    constexpr decltype(auto) map_or_else(std::invocable auto&& def, std::invocable<value_type const&> auto&& f) const& {
        return is_just()
            ? std::invoke(std::forward<decltype(f)>(f), unwrap())
            : std::invoke(std::forward<decltype(def)>(def));
    }

    constexpr decltype(auto) map_or_else(std::invocable auto&& def, std::invocable<value_type&&> auto&& f) && {
        return is_just()
            ? std::invoke(std::forward<decltype(f)>(f), std::move(unwrap()))
            : std::invoke(std::forward<decltype(def)>(def));
    }

    constexpr value_type& expect(std::string_view msg) & {
        if (is_nothing()) PANIC("{}", msg);
        return unwrap();
    }

    constexpr value_type const& expect(std::string_view msg) const& {
        if (is_just()) {
            return unwrap();
        }
        else {
            PANIC("{}", msg);
        }
    }

    constexpr value_type&& expect(std::string_view msg) && {
        if (is_just()) {
            return std::move(unwrap());
        }
        else {
            PANIC("{}", msg);
        }
    }

    constexpr auto filter(std::predicate<value_type&> auto&& predicate) & {
        return is_just() && std::invoke(std::forward<decltype(predicate)>(predicate), unwrap())
            ? maybe<T>(unwrap())
            : nothing;
    }

    constexpr auto filter(std::predicate<value_type const&> auto&& predicate) const& {
        return is_just() && std::invoke(std::forward<decltype(predicate)>(predicate), unwrap())
            ? maybe<T>(unwrap())
            : nothing;
    }

    constexpr auto filter(std::predicate<value_type&> auto&& predicate) && {
        return is_just() && std::invoke(std::forward<decltype(predicate)>(predicate), unwrap())
            ? maybe<T>(std::move(unwrap()))
            : nothing;
    }

    constexpr auto ok_or(auto&& err) const& -> result<T, decltype(err)> {
        if ( is_just() ) return success{unwrap()};
        else return failure{std::forward<decltype(err)>(err)};
    }

    constexpr auto ok_or(auto&& err) && -> result<T, decltype(err)> {
        if ( is_just() ) return success{std::move(unwrap())};
        else return failure{std::forward<decltype(err)>(err)};
    }

    constexpr auto ok_or() const& -> result<T> {
        if ( is_just() ) return success{unwrap()};
        else return failure<>{};
    }

    constexpr auto ok_or() && -> result<T> {
        if ( is_just() ) return success{std::move(unwrap())};
        else return failure<>{};
    }

    constexpr auto ok_or_else(std::invocable auto&& err) const& -> result<T, std::invoke_result_t<decltype(err)>> {
        if (is_just()) return success{unwrap()};
        else return failure{std::invoke(std::forward<decltype(err)>(err))};
    }

    constexpr auto ok_or_else(std::invocable auto&& err) && -> result<T, std::invoke_result_t<decltype(err)>> {
        if (is_just()) return success{std::move(unwrap())};
        else return failure{std::invoke(std::forward<decltype(err)>(err))};
    }

    constexpr auto conj(decay_satisfy<is_maybe> auto&& rhs) const {
        return is_just() ? std::forward<decltype(rhs)>(rhs) : nothing;
    }

    template <class U>
    constexpr maybe<U> operator&&(maybe<U> const& rhs) const {
        return this->conj(rhs);
    }

    constexpr maybe<T> disj(maybe<T> const& rhs) const {
        return is_nothing() ? rhs : *this;
    }

    constexpr maybe<T> operator||(maybe<T> const& rhs) const {
        return this->disj(rhs);
    }

    constexpr maybe<T> xdisj(maybe<T> const& rhs) const {
        return is_just() ^ rhs.is_just() ? is_just() ? *this : rhs : nothing;
    }

    constexpr maybe<T> operator^(maybe<T> const& rhs) const {
        return this->xdisj(rhs);
    }

    constexpr auto and_then(std::regular_invocable<T&> auto&& f) & requires (is_maybe<std::decay_t<std::invoke_result_t<decltype(f), T&>>>::value) {
        return is_just() ? std::invoke(std::forward<decltype(f)>(f), unwrap()) : nothing;
    }

    constexpr auto and_then(std::regular_invocable<T const&> auto&& f) const& requires (is_maybe<std::decay_t<std::invoke_result_t<decltype(f), T const&>>>::value) {
        return is_just() ? std::invoke(std::forward<decltype(f)>(f), unwrap()) : nothing;
    }

    constexpr auto and_then(std::regular_invocable<T&&> auto&& f) && requires (is_maybe<std::decay_t<std::invoke_result_t<decltype(f), T&&>>>::value) {
        return is_just() ? std::invoke(std::forward<decltype(f)>(f), static_cast<T&&>(unwrap())) : nothing;
    }

    constexpr auto or_else(std::regular_invocable auto&& f) & requires (is_maybe_with<std::decay_t<std::invoke_result_t<decltype(f)>>, T>::value) {
        return is_just() ? just(unwrap()) : std::invoke(std::forward<decltype(f)>(f));
    }

    constexpr auto or_else(std::regular_invocable auto&& f) const& requires (is_maybe_with<std::decay_t<std::invoke_result_t<decltype(f)>>, T>::value) {
        return is_just() ? just(unwrap()) : std::invoke(std::forward<decltype(f)>(f));
    }

    constexpr auto or_else(std::regular_invocable auto&& f) && requires (is_maybe_with<std::decay_t<std::invoke_result_t<decltype(f)>>, T>::value) {
        return is_just() ? just(static_cast<T&&>(unwrap())) : std::invoke(std::forward<decltype(f)>(f));
    }

    constexpr auto transpose() const& requires (is_result_v<std::decay_t<T>>) {
        using result_t = basic_result<std::decay_t<T>::mutability_v, maybe<typename std::decay_t<T>::ok_type>, typename std::decay_t<T>::err_type>;
        return this->is_nothing()
            ? result_t{success{nothing}}
            : this->unwrap().is_ok()
                ? result_t{in_place_ok, std::in_place, this->unwrap().unwrap()}
                : result_t{in_place_err, this->unwrap().unwrap_err()};
    }

    constexpr decltype(auto) and_finally(std::invocable<value_type&> auto&& f) & {
        if (is_just()) std::invoke(std::forward<decltype(f)>(f), unwrap());
    }

    constexpr decltype(auto) and_finally(std::invocable<value_type const&> auto&& f) const& {
        if (is_just()) std::invoke(std::forward<decltype(f)>(f), unwrap());
    }

    constexpr decltype(auto) and_finally(std::invocable<T&&> auto&& f) && {
        if (is_just()) std::invoke(std::forward<decltype(f)>(f), static_cast<T&&>(unwrap()));
    }

    constexpr decltype(auto) or_finally(std::invocable auto&& f) {
        if (is_nothing()) std::invoke(std::forward<decltype(f)>(f));
    }

    template <class F>
    constexpr
    std::enable_if_t<
        std::disjunction_v<
            std::is_invocable<F, value_type&>,
            std::is_invocable<F>>,
    maybe&>
    and_peek(F&& f) &
    {
        if (is_just()) {
            if constexpr (std::is_invocable_v<F, value_type&>) {
                std::invoke(std::forward<F>(f), unwrap());
            }
            else {
                std::invoke(std::forward<F>(f));
            }
        }
        return *this;
    }

    template <class F>
    std::enable_if_t<
        std::disjunction_v<
            std::is_invocable<F&&, value_type const&>,
            std::is_invocable<F&&>>,
    maybe const&>
    and_peek(F&& f) const&
    {
        if constexpr (std::is_invocable_v<F&&, value_type const&>) {
            return is_just() ? std::invoke(std::forward<F>(f), unwrap())
                             : *this;
        }
        else {
            return is_just() ? std::invoke(std::forward<F>(f))
                             : *this;
        }
    }

    template <class F>
    std::enable_if_t<
        std::disjunction_v<
            std::is_invocable<F&&, value_type&&>,
            std::is_invocable<F&&>>,
    maybe&&>
    and_peek(F&& f) &&
    {
        if constexpr (std::is_invocable_v<F&&, value_type&&>) {
            return is_just() ? std::invoke(std::forward<F>(f), std::move(unwrap()))
                             : *this;
        }
        else {
            return is_just() ? std::invoke(std::forward<F>(f))
                             : *this;
        }
    }

    template <class F>
    constexpr
    std::enable_if_t<
        std::is_invocable_v<F&&>,
    maybe&>
    or_peek(F&& f) &
    {
        if (is_nothing()) std::invoke(std::forward<F>(f));
        return *this;
    }

    template <class F>
    constexpr
    std::enable_if_t<
        std::is_invocable_v<F&&>,
    maybe const&>
    or_peek(F&& f) const &
    {
        if (is_nothing()) std::invoke(std::forward<F>(f));
        return *this;
    }

    template <class F>
    constexpr
    std::enable_if_t<
        std::is_invocable_v<F&&>,
    maybe&&>
    or_peek(F&& f) &&
    {
        if (is_nothing()) std::invoke(std::forward<F>(f));
        return std::move(*this);
    }

  template <std::totally_ordered_with<T> U>
  constexpr std::strong_ordering operator<=>(maybe<U> const& other) const {
    if (this->is_just() && other.is_nothing()) return std::strong_ordering::greater;
    else if (this->is_nothing() && other.is_just()) return std::strong_ordering::less;
    else if (this->is_just() && other.is_just()) {
      if (this->unwrap() < other.unwrap()) return std::strong_ordering::less;
      if (this->unwrap() > other.unwrap()) return std::strong_ordering::greater;
      else return std::strong_ordering::equivalent;
    }
    else {
      return std::strong_ordering::equivalent;
    }
  }

  template <std::totally_ordered_with<T> U>
  constexpr std::strong_ordering operator<=>(just_t<U> const& other) const {
    if (this->is_nothing()) return std::strong_ordering::less;
    else {
      if (this->unwrap() < other.get()) return std::strong_ordering::less;
      if (this->unwrap() > other.get()) return std::strong_ordering::greater;
      else return std::strong_ordering::equivalent;
    }
  }

  constexpr std::strong_ordering operator<=>(nothing_t const& other) const
  { return this->is_nothing() ? std::strong_ordering::equivalent : std::strong_ordering::greater; }

  constexpr std::strong_ordering operator<=>(value_type const& other) const
  { return *this <=> just(other); }

};

template <class T> requires (!pointer_like<T> && !is_just<T>::value)
maybe(T&&) -> maybe<T>;
template <pointer_like T>
maybe(T&&) -> maybe<deref_type_t<std::decay_t<T>>>;

/// @brief
///   ostream output operator for maybe<T>
///
/// @constrains
///   Format<T>;
///
/// @note
///   Output its contained value with pretty format, and is used by `operator<<` found by ADL.
template <display T>
std::ostream&
operator<<(std::ostream& os, maybe<T> const& may) {
    using namespace std::literals;
    return may.is_just() ? os << fmt::format("just({})", as_display(may.unwrap()).as_str())
                         : os << "nothing"sv;
}

}
#endif
