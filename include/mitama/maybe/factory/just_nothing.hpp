#ifndef MITAMA_MAYBE_FACTORY_JUST_HPP
#define MITAMA_MAYBE_FACTORY_JUST_HPP
#include <mitama/mitamagic/is_interface_of.hpp>
#include <mitama/maybe/fwd/maybe_fwd.hpp>
#include <mitama/result/detail/meta.hpp>
#include <mitama/result/traits/impl_traits.hpp>
#include <mitama/concepts/display.hpp>
#include <mitama/concepts/satisfy.hpp>

#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/hana/functional/overload_linearly.hpp>

#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <concepts>

namespace mitama::_just_detail {
    template<class T=void>
    struct forward_mode {};
}

namespace mitama {

constexpr bool operator==(const nothing_t, const nothing_t) { return true; }
constexpr bool operator!=(const nothing_t, const nothing_t) { return false; }
constexpr bool operator< (const nothing_t, const nothing_t) { return false; }
constexpr bool operator> (const nothing_t, const nothing_t) { return false; }
constexpr bool operator<=(const nothing_t, const nothing_t) { return true; }
constexpr bool operator>=(const nothing_t, const nothing_t) { return true; }

inline std::ostream& operator<<(std::ostream& os, nothing_t) { return os << "nothing"; }

template <class>
struct is_just: std::false_type {};

template <class T>
struct is_just<just_t<T>>: std::true_type {};

template <class, class>
struct is_just_with: std::false_type {};

template <class T>
struct is_just_with<just_t<T>, T>: std::true_type {};

/// class just:
/// The main use of this class is to propagate some value to the constructor of the maybe class.
template <class T>
class [[nodiscard("warning: unused result which must be used")]] just_t<T>
{
    template <class...> friend class just_t;
    T x;
public:
    using type = T;

    constexpr just_t() requires std::same_as<T, std::monostate> = default;

    template <std::constructible_from<T> U> requires (!std::same_as<U, just_t>)
    constexpr explicit(!(std::constructible_from<T, U> && std::convertible_to<U, T>))
    just_t(U&& u)
        noexcept(std::is_nothrow_constructible_v<T, U>)
        : x(std::forward<U>(u)) {}

    template <std::constructible_from<T> U> requires (!std::same_as<T, U>)
    explicit(!(std::constructible_from<T, U> && std::convertible_to<U, T>))
    constexpr just_t(const just_t<U> &t) noexcept(std::is_nothrow_constructible_v<T, const U&>)
        : x(t.get()) {}

    template <std::constructible_from<T> U> requires (!std::same_as<T, U>)
    explicit(!(std::constructible_from<T, U> && std::convertible_to<U, T>))
    constexpr just_t(just_t<U> &&t) noexcept(std::is_nothrow_constructible_v<T, U&&>)
        : x(static_cast<U&&>(t.get())) {}

    template <class... Args> requires std::constructible_from<T, Args&&...>
    explicit constexpr just_t(std::in_place_t, Args && ... args)
        noexcept(std::is_nothrow_constructible_v<T, Args&&...>)
        : x(std::forward<Args>(args)...) {}

    template <class U, class... Args> requires std::constructible_from<T, std::initializer_list<U>, Args&&...>
    explicit constexpr just_t(std::in_place_t, std::initializer_list<U> il, Args && ... args)
        noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>, Args&&...>)
        : x(il, std::forward<Args>(args)...) {}

    friend constexpr bool
    operator==(const nothing_t, just_t const&) { return false; }

    friend constexpr bool
    operator==(just_t const&, const nothing_t) { return false; }

    template <std::equality_comparable_with<T> U>
    constexpr bool
    operator==(just_t<U> const& rhs) const {
        return this->x == rhs.get();
    }

    template <std::equality_comparable_with<T> U>
    friend bool
    operator==(maybe<U> const& lhs, just_t const& rhs) {
        return lhs && (lhs.unwrap() == rhs.get());
    }

    template <std::equality_comparable_with<T> U>
    friend bool
    operator==(just_t const& lhs, maybe<U> const& rhs) {
        return rhs && (lhs.get() == rhs.unwrap());
    }

    friend constexpr bool
    operator!=(const nothing_t , just_t const&) { return true; }

    friend constexpr bool
    operator!=(just_t const&, const nothing_t) { return true; }

    template <std::equality_comparable_with<T> U>
    constexpr bool
    operator!=(just_t<U> const& rhs) const{
        return !(this->x == rhs.get());
    }

    template <std::equality_comparable_with<T> U>
    friend bool
    operator!=(maybe<U> const& lhs, just_t const& rhs) {
        return lhs && !(lhs.unwrap() == rhs);
    }

    template <std::equality_comparable_with<T> U>
    friend bool
    operator!=(just_t const& lhs, maybe<U> const& rhs) {
        return rhs && !(lhs == rhs.unwrap());
    }

    friend constexpr bool
    operator<(const nothing_t , just_t const&) { return true; }

    friend constexpr bool
    operator<(just_t const&, const nothing_t) { return false; }

    template <std::totally_ordered_with<T> U>
    constexpr bool
    operator<(just_t<U> const& rhs) const{
        return this->x < rhs.get();
    }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator<(just_t const& lhs, maybe<U> const& rhs) {
        return rhs ? lhs.get() < rhs.unwrap() : false;
    }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator<(maybe<U> const& lhs, just_t const& rhs) {
        return lhs ? lhs.unwrap() < rhs.get() : true;
    }

    friend constexpr bool
    operator<=(const nothing_t , just_t const&) { return true; }

    friend constexpr bool
    operator<=(just_t const&, const nothing_t) { return false; }

    template <std::totally_ordered_with<T> U>
    constexpr bool
    operator<=(just_t<U> const& rhs) const{
        return this->x < rhs.get() || this->x == rhs.get() ;
    }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator<=(just_t const& lhs, maybe<U> const& rhs) {
        return rhs ? (lhs.get() < rhs.unwrap() || lhs.get() == rhs.unwrap()) : false;
    }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator<=(maybe<U> const& lhs, just_t const& rhs) {
        return lhs ? (lhs.unwrap() < rhs.get() || lhs.unwrap() == rhs.get()) : true;
    }

    friend constexpr bool
    operator>(const nothing_t , just_t const&) { return false; }

    friend constexpr bool
    operator>(just_t const&, const nothing_t) { return true; }

    template <std::totally_ordered_with<T> U>
    constexpr bool
    operator>(just_t<U> const& rhs) const{ return rhs < *this; }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator>(just_t const& lhs, maybe<U> const& rhs) { return rhs < lhs; }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator>(maybe<U> const& lhs, just_t const& rhs) { return rhs < lhs; }

    friend constexpr bool
    operator>=(const nothing_t , just_t const&) { return false; }

    friend constexpr bool
    operator>=(just_t const&, const nothing_t) { return true; }

    template <std::totally_ordered_with<T> U>
    constexpr bool
    operator>=(just_t<U> const& rhs) const { return rhs <= *this; }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator>=(just_t const& lhs, maybe<U> const& rhs) { return rhs <= lhs; }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator>=(maybe<U> const& lhs, just_t const& rhs) { return rhs <= lhs; }

    constexpr T& get() & { return x; }
    constexpr T const& get() const& { return x; }
    constexpr T&& get() && { return std::move(x); }
};

template <class T>
class [[nodiscard("warning: unused result which must be used")]] just_t<T&>
{
    template <class...> friend class just_t;
    std::reference_wrapper<T> x;
public:
    using type = T&;

    constexpr just_t() = delete;

    explicit constexpr just_t(T& ref) : x(ref) {}
    explicit constexpr just_t(std::in_place_t, T& ref) : x(ref) {}

    template <class Derived> requires (std::is_base_of_v<T, Derived>)
    explicit constexpr just_t(Derived& derived) : x(derived) {}
    template <class Derived> requires (std::is_base_of_v<T, Derived>)
    explicit constexpr just_t(std::in_place_t, Derived& derived) : x(derived) {}

    explicit constexpr just_t(just_t &&) = default;
    explicit constexpr just_t(just_t const&) = default;
    constexpr just_t& operator=(just_t &&) = default;
    constexpr just_t& operator=(just_t const&) = default;

    friend constexpr bool
    operator==(const nothing_t, just_t const&) { return false; }

    friend constexpr bool
    operator==(just_t const&, const nothing_t) { return false; }

    template <std::equality_comparable_with<T> U>
    constexpr bool
    operator==(just_t<U> const& rhs) const {
        return this->x == rhs.get();
    }

    template <std::equality_comparable_with<T> U>
    friend bool
    operator==(maybe<U> const& lhs, just_t const& rhs) {
        return lhs && (lhs.unwrap() == rhs.get());
    }

    template <std::equality_comparable_with<T> U>
    friend bool
    operator==(just_t const& lhs, maybe<U> const& rhs) {
        return rhs && (lhs.get() == rhs.unwrap());
    }

    friend constexpr bool
    operator!=(const nothing_t , just_t const&) { return true; }

    friend constexpr bool
    operator!=(just_t const&, const nothing_t) { return true; }

    template <std::equality_comparable_with<T> U>
    constexpr bool
    operator!=(just_t<U> const& rhs) const{
        return !(this->x == rhs.get());
    }

    template <std::equality_comparable_with<T> U>
    friend bool
    operator!=(maybe<U> const& lhs, just_t const& rhs) {
        return lhs && !(lhs.unwrap() == rhs);
    }

    template <std::equality_comparable_with<T> U>
    friend bool
    operator!=(just_t const& lhs, maybe<U> const& rhs) {
        return rhs && !(lhs == rhs.unwrap());
    }

    friend constexpr bool
    operator<(const nothing_t , just_t const&) { return true; }

    friend constexpr bool
    operator<(just_t const&, const nothing_t) { return false; }

    template <std::totally_ordered_with<T> U>
    constexpr bool
    operator<(just_t<U> const& rhs) const{
        return this->x < rhs.get();
    }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator<(just_t const& lhs, maybe<U> const& rhs) {
        return rhs ? lhs.get() < rhs.unwrap() : false;
    }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator<(maybe<U> const& lhs, just_t const& rhs) {
        return lhs ? lhs.unwrap() < rhs.get() : true;
    }

    friend constexpr bool
    operator<=(const nothing_t , just_t const&) { return true; }

    friend constexpr bool
    operator<=(just_t const&, const nothing_t) { return false; }

    template <std::totally_ordered_with<T> U>
    constexpr bool
    operator<=(just_t<U> const& rhs) const{
        return this->x < rhs.get() || this->x == rhs.get() ;
    }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator<=(just_t const& lhs, maybe<U> const& rhs) {
        return rhs ? (lhs.get() < rhs.unwrap() || lhs.get() == rhs.unwrap()) : false;
    }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator<=(maybe<U> const& lhs, just_t const& rhs) {
        return lhs ? (lhs.unwrap() < rhs.get() || lhs.unwrap() == rhs.get()) : true;
    }

    friend constexpr bool
    operator>(const nothing_t , just_t const&) { return false; }

    friend constexpr bool
    operator>(just_t const&, const nothing_t) { return true; }

    template <std::totally_ordered_with<T> U>
    constexpr bool
    operator>(just_t<U> const& rhs) const{ return rhs < *this; }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator>(just_t const& lhs, maybe<U> const& rhs) { return rhs < lhs; }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator>(maybe<U> const& lhs, just_t const& rhs) { return rhs < lhs; }

    friend constexpr bool
    operator>=(const nothing_t , just_t const&) { return false; }

    friend constexpr bool
    operator>=(just_t const&, const nothing_t) { return true; }

    template <std::totally_ordered_with<T> U>
    constexpr bool
    operator>=(just_t<U> const& rhs) const { return rhs <= *this; }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator>=(just_t const& lhs, maybe<U> const& rhs) { return rhs <= lhs; }

    template <std::totally_ordered_with<T> U>
    friend constexpr bool
    operator>=(maybe<U> const& lhs, just_t const& rhs) { return rhs <= lhs; }

    constexpr T& get() & { return x.get(); }
    constexpr T const& get() const& { return x.get(); }
    constexpr T& get() && { return x.get(); }
};

template <display T>
inline std::ostream&
operator<<(std::ostream& os, just_t<T> const& x) {
    return os << boost::format("just(%1%)") % as_display( x.get() );
}

template <class Target = void, class... Types>
constexpr auto just(Types&&... v) {
    if constexpr (sizeof...(Types) > 1) {
        return just_t<_just_detail::forward_mode<Target>, Types&&...>{std::forward<Types>(v)...};
    }
    else {
        if constexpr (!std::is_void_v<Target>)
            return just_t<_just_detail::forward_mode<Target>, Types&&...>{std::forward<Types>(v)...};
        else
            return just_t<Types...>{std::forward<Types>(v)...};
    }
}

template <class Target = void, class T, class... Types>
constexpr auto just(std::initializer_list<T> il, Types&&... v) {
    return just_t<_just_detail::forward_mode<Target>, std::initializer_list<T>, Types&&...>{il, std::forward<Types>(v)...};
}

template <class T, class... Args>
class [[nodiscard("warning: unused result which must be used")]] just_t<_just_detail::forward_mode<T>, Args...>
{
    std::tuple<Args...> args;
public:
    constexpr explicit just_t(Args... args): args(std::forward<Args>(args)...) {}

    constexpr auto operator()() && {
        return std::apply([](auto&&... fwd){ return std::forward_as_tuple(std::forward<decltype(fwd)>(fwd)...); }, args);
    }
};
}

#endif
