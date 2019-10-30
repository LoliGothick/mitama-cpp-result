#ifndef MITAMA_RESULT_FACTORY_SUCCESS_HPP
#define MITAMA_RESULT_FACTORY_SUCCESS_HPP
#include <mitama/mitamagic/is_interface_of.hpp>
#include <mitama/result/detail/fwd.hpp>
#include <mitama/result/detail/meta.hpp>
#include <mitama/result/traits/impl_traits.hpp>
#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/hana/functional/overload_linearly.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <mitama/concepts/display.hpp>
#include <concepts>

namespace mitama {
/// class success:
/// The main use of this class is to propagate successful results to the constructor of the result class.
template <class T>
class [[nodiscard("warning: unused result which must be used")]] success
{
  template <class> friend class success;
  T x;

  template <class... Requires>
  using where = std::enable_if_t<std::conjunction_v<Requires...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class U>
  using not_self = std::negation<std::is_same<success, U>>;
public:
  using ok_type = T;

  constexpr success() requires std::same_as<T, std::monostate> = default;

  template <std::constructible_from<T> U> requires (!std::same_as<std::remove_cvref_t<U>, success>)
  constexpr explicit(!std::convertible_to<U&&, T>)
  success(U&& u)
      noexcept(std::is_nothrow_constructible_v<T, U&&>)
      : x(std::forward<U>(u)) {}

  template <std::constructible_from<T> U> requires (!std::same_as<std::remove_cvref_t<U>, success>)
  constexpr explicit(!std::convertible_to<U const&, T>)
  success(const success<U> &t)
      noexcept(std::is_nothrow_constructible_v<T, U const&>)
      : x(t.get()) {}

  template <std::constructible_from<T> U> requires (!std::same_as<std::remove_cvref_t<U>, success>)
  constexpr explicit(!std::convertible_to<U&&, T>)
  success(success<U>&& t)
      noexcept(std::is_nothrow_constructible_v<T, U&&>)
      : x(static_cast<U&&>(t.get())) {}

  template <class... Args> requires (std::constructible_from<T, Args&&...>)
  explicit constexpr
  success(std::in_place_t, Args&&... args)
      noexcept(std::is_nothrow_constructible_v<T, Args...>)
      : x(std::forward<Args>(args)...) {}

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T, T_>::value,
  bool>
  operator==(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_ok() ? rhs.unwrap() == this->x : false;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T, T_>::value,
  bool>
  operator==(success<T_> const& rhs) const {
    return this->x == rhs.x;
  }

  template <class E_>
  constexpr bool
  operator==(failure<E_> const&) const {
    return false;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T, T_>::value,
  bool>
  operator!=(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_ok() ? !(rhs.unwrap() == this->x) : true;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T, T_>::value,
  bool>
  operator!=(success<T_> const& rhs) const {
    return !(this->x == rhs.x);
  }

  template <class E_>
  constexpr bool
  operator!=(failure<E_> const&) const {
    return true;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    meta::is_less_comparable_with<T, T_>::value,
  bool>
  operator<(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_ok() ? this->x < rhs.unwrap() : false;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    meta::is_less_comparable_with<T, T_>::value,
  bool>
  operator<(success<T_> const& rhs) const {
    return this->x < rhs.x;
  }

  template <class E_>
  constexpr bool
  operator<(failure<E_> const&) const {
    return false;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T, T_>::value &&
    meta::is_less_comparable_with<T, T_>::value,
  bool>
  operator<=(basic_result<_mut, T_, E_> const& rhs) const
  {
    return rhs.is_ok() ? (this->x == rhs.unwrap()) || (this->x < rhs.unwrap()) : false;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T, T_>::value &&
    meta::is_less_comparable_with<T, T_>::value,
  bool>
  operator<=(success<T_> const& rhs) const {
    return (this->x == rhs.x) || (this->x < rhs.x);
  }

  template <class E_>
  constexpr bool
  operator<=(failure<E_> const&) const {
    return false;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    meta::is_less_comparable_with<T_, T>::value,
  bool>
  operator>(basic_result<_mut, T_, E_> const& rhs) const
  {
    return rhs.is_ok() ? rhs.unwrap() < this->x : true;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    meta::is_less_comparable_with<T_, T>::value,
  bool>
  operator>(success<T_> const& rhs) const {
    return rhs < *this;
  }

  template <class E_>
  constexpr bool
  operator>(failure<E_> const&) const {
    return true;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T_, T>::value &&
    meta::is_less_comparable_with<T_, T>::value,
  bool>
  operator>=(basic_result<_mut, T_, E_> const& rhs) const
  {
    return rhs.is_ok() ? (rhs.unwrap() == this->x) || (rhs.is_ok() < this->x) : true;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T, T_>::value,
  bool>
  operator>=(success<T_> const& rhs) const {
    return rhs <= *this;
  }

  template <class E_>
  constexpr bool
  operator>=(failure<E_> const&) const {
    return true;
  }

  T& get() & { return x; }
  T const& get() const& { return x; }
  T&& get() && { return std::move(x); }

};

template <class T>
class [[nodiscard("warning: unused result which must be used")]] success<T&>
{
  template <class>
  friend class success;
  std::reference_wrapper<T> x;

  template <class... Requires>
  using where = std::enable_if_t<std::conjunction_v<Requires...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class U>
  using not_self = std::negation<std::is_same<success, U>>;
public:
  using ok_type = T&;

  success() = delete;
  explicit constexpr success(T& ok) : x(ok) {}
  explicit constexpr success(std::in_place_t, T& ok) : x(ok) {}

  template <class Derived, std::enable_if_t<mitamagic::is_interface_of_v<std::decay_t<T>, std::decay_t<Derived>>, bool> = false>
  explicit constexpr success(Derived& derived) : x(derived) {}
  template <class Derived, std::enable_if_t<mitamagic::is_interface_of_v<std::decay_t<T>, std::decay_t<Derived>>, bool> = false>
  explicit constexpr success(std::in_place_t, Derived& derived) : x(derived) {}

  explicit constexpr success(success &&) = default;
  explicit constexpr success(success const&) = default;
  constexpr success& operator=(success &&) = default;
  constexpr success& operator=(success const&) = default;

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T, T_>::value,
  bool>
  operator==(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_ok() ? rhs.unwrap() == this->x : false;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T, T_>::value,
  bool>
  operator==(success<T_> const& rhs) const {
    return this->x == rhs.x;
  }

  template <class E_>
  constexpr bool
  operator==(failure<E_> const&) const {
    return false;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T, T_>::value,
  bool>
  operator!=(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_ok() ? !(rhs.unwrap() == this->x) : true;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T, T_>::value,
  bool>
  operator!=(success<T_> const& rhs) const {
    return !(this->x == rhs.x);
  }

  template <class E_>
  constexpr bool
  operator!=(failure<E_> const&) const {
    return true;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    meta::is_less_comparable_with<T, T_>::value,
  bool>
  operator<(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_ok() ? this->x < rhs.unwrap() : false;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    meta::is_less_comparable_with<T, T_>::value,
  bool>
  operator<(success<T_> const& rhs) const {
    return this->x < rhs.x;
  }

  template <class E_>
  constexpr bool
  operator<(failure<E_> const&) const {
    return false;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T, T_>::value &&
    meta::is_less_comparable_with<T, T_>::value,
  bool>
  operator<=(basic_result<_mut, T_, E_> const& rhs) const
  {
    return rhs.is_ok() ? (this->x == rhs.unwrap()) || (this->x < rhs.unwrap()) : false;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T, T_>::value &&
    meta::is_less_comparable_with<T, T_>::value,
  bool>
  operator<=(success<T_> const& rhs) const {
    return (this->x == rhs.x) || (this->x < rhs.x);
  }

  template <class E_>
  constexpr bool
  operator<=(failure<E_> const&) const {
    return false;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    meta::is_less_comparable_with<T_, T>::value,
  bool>
  operator>(basic_result<_mut, T_, E_> const& rhs) const
  {
    return rhs.is_ok() ? rhs.unwrap() < this->x : true;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    meta::is_less_comparable_with<T_, T>::value,
  bool>
  operator>(success<T_> const& rhs) const {
    return rhs < *this;
  }

  template <class E_>
  constexpr bool
  operator>(failure<E_> const&) const {
    return true;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T_, T>::value &&
    meta::is_less_comparable_with<T_, T>::value,
  bool>
  operator>=(basic_result<_mut, T_, E_> const& rhs) const
  {
    return rhs.is_ok() ? (rhs.unwrap() == this->x) || (rhs.is_ok() < this->x) : true;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<T, T_>::value,
  bool>
  operator>=(success<T_> const& rhs) const {
    return rhs <= *this;
  }

  template <class E_>
  constexpr bool
  operator>=(failure<E_> const&) const {
    return true;
  }

  T& get() & { return x.get(); }
  T const& get() const& { return x.get(); }
  T& get() && { return x.get(); }

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
  template <display T>
  inline std::ostream&
  operator<<(std::ostream& os, success<T> const& ok) {
    return os << boost::format("success(%1%)") % as_display( ok.get() );
  }

}

#endif
