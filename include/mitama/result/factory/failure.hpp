#ifndef MITAMA_RESULT_FACTORY_FAILURE_HPP
#define MITAMA_RESULT_FACTORY_FAILURE_HPP
#include <mitama/mitamagic/is_interface_of.hpp>
#include <mitama/result/detail/fwd.hpp>
#include <mitama/result/detail/meta.hpp>
#include <mitama/concepts/formattable.hpp>
#include <mitama/cmp/ord.hpp>

#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/hana/functional/overload_linearly.hpp>

#include <fmt/core.h>

#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <mitama/concepts/display.hpp>

namespace mitama {
/// class failure:
/// The main use of this class is to propagate unsuccessful results to the constructor of the result class.
template <class E>
class [[nodiscard("warning: unused result which must be used")]] failure
  : public cmp::result_ord<failure<E>>
  , public cmp::ord<failure<E>, success>
  , public cmp::ord<failure<E>, failure>
{
  template <class> friend class failure;
  E x;
public:
  using err_type = E;

  constexpr failure() requires std::same_as<E, std::monostate> = default;

  template <std::constructible_from<E> F> requires (!std::same_as<std::remove_cvref_t<F>, failure>)
  constexpr explicit(!std::convertible_to<F&&, E>)
  failure(F&& u) // NOLINT(google-explicit-constructor,bugprone-forwarding-reference-overload)
      noexcept(std::is_nothrow_constructible_v<E, F&&>)
      : x(std::forward<F>(u)) {}

  template <std::constructible_from<E> F> requires (!std::same_as<std::remove_cvref_t<F>, failure>)
  constexpr explicit(!std::convertible_to<F&, E>)
  failure(const failure<F> &t) // NOLINT(google-explicit-constructor)
      noexcept(std::is_nothrow_constructible_v<E, F&>)
      : x(t.get()) {}

  template <std::constructible_from<E> F> requires (!std::same_as<std::remove_cvref_t<F>, failure>)
  constexpr explicit(!std::convertible_to<F&&, E>)
  failure(failure<F>&& t) // NOLINT(google-explicit-constructor)
      noexcept(std::is_nothrow_constructible_v<E, F&&>)
      : x(static_cast<F&&>(t.get())) {}

  template <class... Args> requires (std::constructible_from<E, Args&&...>)
  explicit constexpr
  failure(std::in_place_t, Args && ... args)
      noexcept(std::is_nothrow_constructible_v<E, Args...>)
      : x(std::forward<Args>(args)...) {}

  constexpr E& get() & { return x; }
  constexpr E const& get() const& { return x; }
  constexpr E&& get() && { return std::move(x); }

  template <mutability _, class T, std::totally_ordered_with<E> F>
  constexpr std::strong_ordering operator<=>(basic_result<_, T, F> const& other) const {
    if (other.is_ok()) return std::strong_ordering::less;
    else {
      if (this->get() < other.unwrap_err()) return std::strong_ordering::less;
      if (this->get() > other.unwrap_err()) return std::strong_ordering::greater;
      else return std::strong_ordering::equivalent;
    }
  }

  template <class T>
  constexpr std::strong_ordering operator<=>(success<T> const&) const {
    return std::strong_ordering::less;
  }

  template <std::totally_ordered_with<E> F>
  constexpr std::strong_ordering operator<=>(failure<F> const& other) const {
    if (this->get() < other.get()) return std::strong_ordering::less;
    if (this->get() > other.get()) return std::strong_ordering::greater;
    else return std::strong_ordering::equivalent;
  }
};

template <class E>
class [[nodiscard("warning: unused result which must be used")]] failure<E&>
  : public cmp::result_ord<failure<E&>>
  , public cmp::ord<failure<E&>, success>
  , public cmp::ord<failure<E&>, failure>
{
  template <class>
  friend class failure;
  std::reference_wrapper<E> x;

  template <class... Requires>
  using where = std::enable_if_t<std::conjunction_v<Requires...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class U>
  using not_self = std::negation<std::is_same<failure, U>>;
public:
  using err_type = E&;

  failure() = delete;
  explicit constexpr failure(E& err) : x(err) {}
  explicit constexpr failure(std::in_place_t, E& err) : x(err) {}

  template <class Derived> requires (std::is_base_of_v<E, Derived>)
  explicit constexpr failure(Derived& derived) : x(derived) {}
  template <class Derived> requires (std::is_base_of_v<E, Derived>)
  explicit constexpr failure(std::in_place_t, Derived& derived) : x(derived) {}

  constexpr failure(failure &&) noexcept = default;
  constexpr failure(failure const&) = default;
  constexpr failure& operator=(failure &&) noexcept = default;
  constexpr failure& operator=(failure const&) = default;

  constexpr E& get() & { return x.get(); }
  constexpr E const& get() const& { return x.get(); }
  constexpr E& get() && { return x.get(); }

  template <mutability _, class T, std::totally_ordered_with<E> F>
  constexpr std::strong_ordering operator<=>(basic_result<_, T, F> const& other) const {
    if (other.is_ok()) return std::strong_ordering::less;
    else {
      if (this->get() < other.unwrap_err()) return std::strong_ordering::less;
      if (this->get() > other.unwrap_err()) return std::strong_ordering::greater;
      else return std::strong_ordering::equivalent;
    }
  }

  template <class T>
  constexpr std::strong_ordering operator<=>(success<T> const& other) const {
    return std::strong_ordering::less;
  }

  template <std::totally_ordered_with<E> F>
  constexpr std::strong_ordering operator<=>(failure<F> const& other) const {
    if (this->get() < other.get()) return std::strong_ordering::less;
    if (this->get() > other.get()) return std::strong_ordering::greater;
    else return std::strong_ordering::equivalent;
  }
};

  /// @brief
  ///   ostream output operator
  ///
  /// @constrains
  ///   Format<T>;
  ///   Format<E>
  ///
  /// @note
  ///   Output its contained value with pretty format, and is used by `operator<<` found by ADL.
  template <display E>
  inline std::ostream&
  operator<<(std::ostream& os, failure<E> const& err) {
    return os << fmt::format("failure({})", as_display(err.get()).as_str());
  }

}

#endif
