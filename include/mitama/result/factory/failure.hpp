#ifndef MITAMA_RESULT_FACTORY_FAILURE_HPP
#define MITAMA_RESULT_FACTORY_FAILURE_HPP
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

namespace mitama {
/// class failure:
/// The main use of this class is to propagate unsuccessful results to the constructor of the result class.
template <class E>
class [[nodiscard("warning: unused result which must be used")]] failure
{
  template <class> friend class failure;
  E x;
public:
  using err_type = E;

  constexpr failure() requires std::same_as<E, std::monostate> = default;

  template <std::constructible_from<E> F> requires (!std::same_as<std::remove_cvref_t<F>, failure>)
  constexpr explicit(!std::convertible_to<F&&, E>)
  failure(F&& u)
      noexcept(std::is_nothrow_constructible_v<E, F&&>)
      : x(std::forward<F>(u)) {}

  template <std::constructible_from<E> F> requires (!std::same_as<std::remove_cvref_t<F>, failure>)
  constexpr explicit(!std::convertible_to<F&, E>)
  failure(const failure<F> &t)
      noexcept(std::is_nothrow_constructible_v<E, F&>)
      : x(t.get()) {}

  template <std::constructible_from<E> F> requires (!std::same_as<std::remove_cvref_t<F>, failure>)
  constexpr explicit(!std::convertible_to<F&&, E>)
  failure(failure<F>&& t)
      noexcept(std::is_nothrow_constructible_v<E, F&&>)
      : x(static_cast<F&&>(t.get())) {}

  template <class... Args> requires (std::constructible_from<E, Args&&...>)
  explicit constexpr
  failure(std::in_place_t, Args && ... args)
      noexcept(std::is_nothrow_constructible_v<E, Args...>)
      : x(std::forward<Args>(args)...) {}

  template <mutability _mut, class T_, std::equality_comparable_with<E> E_>
  constexpr bool operator==(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_err() ? rhs.unwrap_err() == this->x : false;
  }

  template <class T_>
  constexpr bool operator==(success<T_> const&) const { return false; }

  template <std::equality_comparable_with<E> E_>
  constexpr bool operator==(failure<E_> const& rhs) const
  { return this->x == rhs.x; }

  template <mutability _mut, class T_, std::equality_comparable_with<E> E_>
  constexpr bool operator!=(basic_result<_mut, T_, E_> const& rhs) const
  { return !(*this == rhs); }

  template <class T_>
  constexpr bool operator!=(success<T_> const&) const
  { return false; }

  template <std::equality_comparable_with<E> E_>
  constexpr bool operator!=(failure<E_> const& rhs) const
  { return !(this->x == rhs.x); }

  template <mutability _mut, class T_, std::totally_ordered_with<E> E_>
  constexpr bool operator<(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_err() ? this->x < rhs.unwrap_err() : true;
  }

  template <class T_>
  constexpr bool operator<(success<T_> const&) const
  { return true; }

  template <std::totally_ordered_with<E> E_>
  constexpr bool operator<(failure<E_> const& rhs) const
  { return this->x < rhs.x; }

  template <mutability _mut, class T_, std::totally_ordered_with<E> E_>
  constexpr bool operator<=(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_err() ? (this->x == rhs.unwrap_err()) || (this->x < rhs.unwrap_err()) : true;
  }

  template <class T_>
  constexpr bool operator<=(success<T_> const&) const
  { return true; }

  template <std::totally_ordered_with<E> E_>
  constexpr bool operator<=(failure<E_> const& rhs) const {
    return (*this == rhs) || (*this < rhs);
  }

  template <mutability _mut, class T_, std::totally_ordered_with<E> E_>
  constexpr bool operator>(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_err() ? rhs.unwrap_err() < this->x : false;
  }

  template <class T_>
  constexpr bool operator>(success<T_> const&) const
  { return false; }

  template <std::totally_ordered_with<E> E_>
  constexpr bool operator>(failure<E_> const& rhs) const
  { return rhs < *this; }

  template <mutability _mut, class T_, std::totally_ordered_with<E> E_>
  constexpr bool operator>=(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_err() ? (rhs.unwrap_err() == this->x) || (rhs.unwrap_err() < this->x) : false;
  }

  template <class T_>
  constexpr bool operator>=(success<T_> const&) const { return false; }

  template <class E_>
  constexpr bool operator>=(failure<E_> const& rhs) const
  { return rhs <= *this; }

  constexpr E& get() & { return x; }
  constexpr E const& get() const& { return x; }
  constexpr E&& get() && { return std::move(x); }

};

template <class E>
class [[nodiscard("warning: unused result which must be used")]] failure<E&>
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

  explicit constexpr failure(failure &&) = default;
  explicit constexpr failure(failure const&) = default;
  constexpr failure& operator=(failure &&) = default;
  constexpr failure& operator=(failure const&) = default;

  template <class T_>
  constexpr bool operator==(success<T_> const&) const { return false; }

  template <std::equality_comparable_with<E> E_>
  constexpr bool operator==(failure<E_> const& rhs) const
  { return this->x == rhs.x; }

  template <mutability _mut, class T_, std::equality_comparable_with<E> E_>
  constexpr bool operator!=(basic_result<_mut, T_, E_> const& rhs) const
  { return !(*this == rhs); }

  template <class T_>
  constexpr bool operator!=(success<T_> const&) const
  { return false; }

  template <std::equality_comparable_with<E> E_>
  constexpr bool operator!=(failure<E_> const& rhs) const
  { return !(this->x == rhs.x); }

  template <mutability _mut, class T_, std::totally_ordered_with<E> E_>
  constexpr bool operator<(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_err() ? this->x < rhs.unwrap_err() : true;
  }

  template <class T_>
  constexpr bool operator<(success<T_> const&) const
  { return true; }

  template <std::totally_ordered_with<E> E_>
  constexpr bool operator<(failure<E_> const& rhs) const
  { return this->x < rhs.x; }

  template <mutability _mut, class T_, std::totally_ordered_with<E> E_>
  constexpr bool operator<=(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_err() ? (this->x == rhs.unwrap_err()) || (this->x < rhs.unwrap_err()) : true;
  }

  template <class T_>
  constexpr bool operator<=(success<T_> const&) const
  { return true; }

  template <std::totally_ordered_with<E> E_>
  constexpr bool operator<=(failure<E_> const& rhs) const {
    return (*this == rhs) || (*this < rhs);
  }

  template <mutability _mut, class T_, std::totally_ordered_with<E> E_>
  constexpr bool operator>(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_err() ? rhs.unwrap_err() < this->x : false;
  }

  template <class T_>
  constexpr bool operator>(success<T_> const&) const
  { return false; }

  template <std::totally_ordered_with<E> E_>
  constexpr bool operator>(failure<E_> const& rhs) const
  { return rhs < *this; }

  template <mutability _mut, class T_, std::totally_ordered_with<E> E_>
  constexpr bool operator>=(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_err() ? (rhs.unwrap_err() == this->x) || (rhs.unwrap_err() < this->x) : false;
  }

  template <class T_>
  constexpr bool operator>=(success<T_> const&) const { return false; }

  template <class E_>
  constexpr bool operator>=(failure<E_> const& rhs) const
  { return rhs <= *this; }

  constexpr E& get() & { return x.get(); }
  constexpr E const& get() const& { return x.get(); }
  constexpr E& get() && { return x.get(); }

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
    return os << boost::format("failure(%1%)") % as_display( err.get() );
  }

}

#endif
