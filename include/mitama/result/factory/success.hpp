#ifndef MITAMA_RESULT_FACTORY_SUCCESS_HPP
#define MITAMA_RESULT_FACTORY_SUCCESS_HPP
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
#include <concepts>

namespace mitama {
/// class success:
/// The main use of this class is to propagate successful results to the constructor of the result class.
template <class T>
class [[nodiscard("warning: unused result which must be used")]] success
  : public cmp::result_ord<success<T>>
  , public cmp::ord<success<T>, success>
  , public cmp::ord<success<T>, failure>
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
  success(U&& u) // NOLINT(google-explicit-constructor,bugprone-forwarding-reference-overload)
      noexcept(std::is_nothrow_constructible_v<T, U&&>)
      : x(std::forward<U>(u)) {}

  template <std::constructible_from<T> U> requires (!std::same_as<std::remove_cvref_t<U>, success>)
  constexpr explicit(!std::convertible_to<U const&, T>)
  success(const success<U> &t) // NOLINT(google-explicit-constructor)
      noexcept(std::is_nothrow_constructible_v<T, U const&>)
      : x(t.get()) {}

  template <std::constructible_from<T> U> requires (!std::same_as<std::remove_cvref_t<U>, success>)
  constexpr explicit(!std::convertible_to<U&&, T>)
  success(success<U>&& t) // NOLINT(google-explicit-constructor)
      noexcept(std::is_nothrow_constructible_v<T, U&&>)
      : x(static_cast<U&&>(t.get())) {}

  template <class... Args> requires (std::constructible_from<T, Args&&...>)
  explicit constexpr
  success(std::in_place_t, Args&&... args)
      noexcept(std::is_nothrow_constructible_v<T, Args...>)
      : x(std::forward<Args>(args)...) {}

  constexpr T& get() & { return x; }
  constexpr T const& get() const& { return x; }
  constexpr T&& get() && { return std::move(x); }

  template <mutability _, std::totally_ordered_with<T> U, class E>
  constexpr std::strong_ordering operator<=>(basic_result<_, U, E> const& other) const {
    if (other.is_err()) return std::strong_ordering::greater;
    else {
      if (this->get() < other.unwrap()) return std::strong_ordering::less;
      if (this->get() > other.unwrap()) return std::strong_ordering::greater;
      else return std::strong_ordering::equivalent;
    }
  }

  template <std::totally_ordered_with<T> U>
  constexpr std::strong_ordering operator<=>(success<U> const& other) const {
    if (this->get() < other.get()) return std::strong_ordering::less;
    if (this->get() > other.get()) return std::strong_ordering::greater;
    else return std::strong_ordering::equivalent;
  }

  template <class E>
  constexpr std::strong_ordering operator<=>(failure<E> const&) const {
    return std::strong_ordering::greater;
  }

};

template <class T>
class [[nodiscard("warning: unused result which must be used")]] success<T&>
  : public cmp::result_ord<success<T&>>
  , public cmp::ord<success<T&>, success>
  , public cmp::ord<success<T&>, failure>
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

  template <class Derived> requires (std::is_base_of_v<std::remove_cv_t<T>, std::remove_cv_t<Derived>>)
  explicit constexpr success(Derived& derived): x{derived} {}
  template <class Derived> requires (std::is_base_of_v<std::remove_cv_t<T>, std::remove_cv_t<Derived>>)
  explicit constexpr success(std::in_place_t, Derived& derived): x{derived} {}

  constexpr success(success &&) noexcept = default;
  constexpr success(success const&) = default;
  constexpr success& operator=(success &&) noexcept = default;
  constexpr success& operator=(success const&) = default;

  constexpr T& get() & { return x.get(); }
  constexpr T const& get() const& { return x.get(); }
  constexpr T& get() && { return x.get(); }

  template <mutability _, std::totally_ordered_with<T> U, class E>
  constexpr std::strong_ordering operator<=>(basic_result<_, U, E> const& other) const {
    if (other.is_err()) return std::strong_ordering::greater;
    else {
      if (this->get() < other.unwrap()) return std::strong_ordering::less;
      if (this->get() > other.unwrap()) return std::strong_ordering::greater;
      else return std::strong_ordering::equivalent;
    }
  }

  template <std::totally_ordered_with<T> U>
  constexpr std::strong_ordering operator<=>(success<U> const& other) const {
    if (this->get() < other.get()) return std::strong_ordering::less;
    if (this->get() > other.get()) return std::strong_ordering::greater;
    else return std::strong_ordering::equivalent;
  }

  template <class E>
  constexpr std::strong_ordering operator<=>(failure<E> const& other) const {
    return std::strong_ordering::greater;
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
  template <display T>
  inline std::ostream&
  operator<<(std::ostream& os, success<T> const& ok) {
    return os << fmt::format("success({})", as_display(ok.get()).as_str());
  }

}

#endif
