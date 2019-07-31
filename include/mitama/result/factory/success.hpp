#ifndef MITAMA_RESULT_FACTORY_SUCCESS_HPP
#define MITAMA_RESULT_FACTORY_SUCCESS_HPP
#include <mitama/result/detail/fwd.hpp>
#include <mitama/result/detail/meta.hpp>
#include <mitama/result/traits/impl_traits.hpp>
#include <mitama/result/traits/perfect_traits_special_members.hpp>
#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/hana/functional/overload_linearly.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace mitama {
/// class success:
/// The main use of this class is to propagate successful results to the constructor of the result class.
template <class T>
class [[nodiscard]] success
    : private ::mitamagic::perfect_trait_copy_move<
          std::is_copy_constructible_v<std::decay_t<T>>,
          std::conjunction_v<std::is_copy_constructible<std::decay_t<T>>, std::is_copy_assignable<std::decay_t<T>>>,
          std::is_move_constructible_v<std::decay_t<T>>,
          std::conjunction_v<std::is_move_constructible<std::decay_t<T>>, std::is_move_assignable<std::decay_t<T>>>,
          success<T>>
{
  template <class>
  friend class success;
  T x;
  template <mutability, class, class, class>
  friend class basic_result;

  template <class... Requires>
  using where = std::enable_if_t<std::conjunction_v<Requires...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class U>
  using not_self = std::negation<std::is_same<success, U>>;
public:
  using ok_type = T;

  template <class U = T>
  constexpr success(std::enable_if_t<std::is_same_v<std::monostate, U>, std::nullptr_t> = nullptr)
  { /* whatever */ }

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<T, U>,
                  std::is_convertible<U, T>> = required>
  constexpr success(U&&  u) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::forward<U>(u)) {}

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit constexpr success(U&&  u) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::forward<U>(u)) {}

  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, const U &>,
                  std::is_convertible<const U &, T>> = required>
  constexpr success(const success<U> &t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(t.x) {}

  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, const U &>,
                  std::negation<std::is_convertible<const U &, T>>> = required>
  explicit constexpr success(const success<U> &t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(t.x) {}

  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, U&&>,
                  std::is_convertible<U&&, T>> = required>
  constexpr success(success<U> && t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::move(t.x)) {}

  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, U&&>,
                  std::negation<std::is_convertible<U&&, T>>> = required>
  explicit constexpr success(success<U> && t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::move(t.x)) {}

  template <class... Args,
            where<std::is_constructible<T, Args...>> = required>
  explicit constexpr success(std::in_place_t, Args && ... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
      : x(std::forward<Args>(args)...) {}

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    is_comparable_with<T, T_>::value,
  bool>
  operator==(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_ok() ? rhs.unwrap() == this->x : false;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    is_comparable_with<T, T_>::value,
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
    is_comparable_with<T, T_>::value,
  bool>
  operator!=(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_ok() ? !(rhs.unwrap() == this->x) : true;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    is_comparable_with<T, T_>::value,
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
    is_less_comparable_with<T, T_>::value,
  bool>
  operator<(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_ok() ? this->x < rhs.unwrap() : false;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    is_less_comparable_with<T, T_>::value,
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
    is_comparable_with<T, T_>::value &&
    is_less_comparable_with<T, T_>::value,
  bool>
  operator<=(basic_result<_mut, T_, E_> const& rhs) const
  {
    return rhs.is_ok() ? (this->x == rhs.unwrap()) || (this->x < rhs.unwrap()) : false;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    is_comparable_with<T, T_>::value &&
    is_less_comparable_with<T, T_>::value,
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
    is_less_comparable_with<T_, T>::value,
  bool>
  operator>(basic_result<_mut, T_, E_> const& rhs) const
  {
    return rhs.is_ok() ? rhs.unwrap() < this->x : true;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    is_less_comparable_with<T_, T>::value,
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
    is_comparable_with<T_, T>::value &&
    is_less_comparable_with<T_, T>::value,
  bool>
  operator>=(basic_result<_mut, T_, E_> const& rhs) const
  {
    return rhs.is_ok() ? (rhs.unwrap() == this->x) || (rhs.is_ok() < this->x) : true;
  }

  template <class T_>
  constexpr
  std::enable_if_t<
    is_comparable_with<T, T_>::value,
  bool>
  operator>=(success<T_> const& rhs) const {
    return rhs <= *this;
  }

  template <class E_>
  constexpr bool
  operator>=(failure<E_> const&) const {
    return true;
  }

  /// @brief
  ///   ostream output operator
  ///
  /// @requires
  ///   Format<T>;
  ///   Format<E>
  ///
  /// @note
  ///   Output its contained value with pretty format, and is used by `operator<<` found by ADL.
  friend
  std::ostream& operator<<(std::ostream& os, success const& fail) {
    static_assert(trait::formattable<T>::value,
        "Error: trait `formattable<T>` was not satisfied.\n"
        "Hint: Did you forgot some include?\n"
        "Otherwise, please define `operator<<(std::ostream&, T)`.");
    using namespace std::literals::string_literals;
    auto inner_format = boost::hana::fix(boost::hana::overload_linearly(
        [](auto, auto const& x) -> std::enable_if_t<trait::formattable_element<std::decay_t<decltype(x)>>::value, std::string> {
          return boost::hana::overload_linearly(
            [](std::monostate) { return "()"s; },
            [](std::string_view x) { return (boost::format("\"%1%\"") % x).str(); },
            [](auto const& x) { return (boost::format("%1%") % x).str(); })
          (x);        
        },
        [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_dictionary<std::decay_t<decltype(x)>>::value, std::string> {
          if (x.empty()) return "{}"s;
          using std::begin, std::end;
          auto iter = begin(x);
          std::string str = "{"s + (boost::format("%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
          while (++iter != end(x)) {
            str += (boost::format(",%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
          }
          return str += "}";
        },
        [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_range<std::decay_t<decltype(x)>>::value, std::string> {
          if (x.empty()) return "[]"s;
          using std::begin, std::end;
          auto iter = begin(x);
          std::string str = "["s + _fmt(*iter);
          while (++iter != end(x)) {
            str += (boost::format(",%1%") % _fmt(*iter)).str();
          }
          return str += "]";
        },
        [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_tuple<std::decay_t<decltype(x)>>::value, std::string> {
          if constexpr (std::tuple_size_v<std::decay_t<decltype(x)>> == 0) {
            return "()"s;
          }
          else {
            return std::apply(
              [_fmt](auto const& head, auto const&... tail) {
                return "("s + _fmt(head) + ((("," + _fmt(tail))) + ...) + ")"s;
              }, x);
          }
        }));
    return os << boost::format("success(%1%)") % inner_format( fail.x );
  }

};
}

#endif