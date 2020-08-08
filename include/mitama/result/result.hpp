#ifndef MITAMA_RESULT_HPP
#define MITAMA_RESULT_HPP
#include <mitama/result/detail/fwd.hpp>
#include <mitama/result/detail/meta.hpp>
#include <mitama/panic.hpp>
#include <mitama/result/factory/success.hpp>
#include <mitama/result/factory/failure.hpp>
#include <mitama/result/detail/dangling.hpp>
#include <mitama/concepts/dereferencable.hpp>
#include <mitama/maybe/fwd/maybe_fwd.hpp>
#include <mitama/cmp/ord.hpp>

#include <boost/hana/functional/overload.hpp>
#include <boost/hana/functional/overload_linearly.hpp>
#include <boost/hana/functional/fix.hpp>


#include <functional>
#include <optional>
#include <variant>
#include <type_traits>
#include <utility>
#include <string_view>

namespace mitama {

/// @brief class basic_result
/// @param _mutability: enum class value for mutability control
/// @param T: Type of successful value
/// @param E: Type of unsuccessful value
template <mutability _mutability, class T, class E>
  requires (std::is_object_v<std::remove_cvref_t<T>>)
        && (std::is_object_v<std::remove_cvref_t<E>>)
        && (std::negation_v<std::is_array<std::remove_cvref_t<T>>>)
        && (std::negation_v<std::is_array<std::remove_cvref_t<E>>>)
class [[nodiscard("warning: unused result which must be used")]] basic_result
  : public cmp::result_ord<basic_result<_mutability, T, E>>
  , public cmp::ord<basic_result<_mutability, T, E>, success>
  , public cmp::ord<basic_result<_mutability, T, E>, failure>
  , public cmp::rel<basic_result<_mutability, T, E>, T>
{
  /// result storage
  std::variant<success<T>, failure<E>> storage_;
  /// friend accessors
  template <mutability, class, class>
  friend class basic_result;
  /// private aliases
  template <class... Requires>
  using where = std::enable_if_t<std::conjunction_v<Requires...>, std::nullptr_t>;
  static constexpr std::nullptr_t required = nullptr;
  template <mutability _mut, class T_, class E_>
  using not_self = std::negation<std::is_same<basic_result, basic_result<_mut, T_, E_>>>;
public:
  /// associated types
  using ok_type = T;
  using err_type = E;
  using ok_reference_type = std::remove_reference_t<T>&;
  using err_reference_type = std::remove_reference_t<E>&;
  using ok_const_reference_type = std::remove_cvref_t<T> const&;
  using err_const_reference_type = std::remove_cvref_t<E> const&;
  /// mutability
  static constexpr bool is_mut = !static_cast<bool>(_mutability);
  static constexpr mutability mutability_v = _mutability;

  /* Constructors */

  /// default constructor is not permitted
  constexpr basic_result() noexcept = delete;

  /// @brief
  ///   explicit copy construcor for convertible basic_result
  template <mutability _mu, class U, class F>
    requires std::constructible_from<T, U>
          && std::constructible_from<E, F>
  constexpr
  explicit(!(std::convertible_to<U, T> && std::convertible_to<F, E>))
  basic_result(basic_result<_mu, U, F> const& res) {
    if (res.is_ok())
      { this->storage_ = success<T>(res.unwrap()); }
    else
      { this->storage_ = failure<E>(res.unwrap_err()); }
  }

  /// @brief
  ///   explicit move construcor for convertible basic_result
  template <mutability _mu, class U, class F>
    requires std::constructible_from<T, U>
          && std::constructible_from<E, F>
  constexpr
  explicit(!(std::convertible_to<U, T> && std::convertible_to<F, E>))
  basic_result(basic_result<_mu, U, F>&& res) {
    if (res.is_ok())
      { this->storage_ = success<T>(res.unwrap()); }
    else
      { this->storage_ = failure<E>(res.unwrap_err()); }
  }

  /// @brief
  ///   copy assignment operator for convertible basic_result
  template <mutability _mu, class U, class F>
    requires std::constructible_from<T, U>
          && std::constructible_from<E, F>
  constexpr basic_result& operator=(basic_result<_mu, U, F> const& res)
  {
    static_assert(is_mut_v<_mutability>, "Error: assignment to immutable result");
    if (res.is_ok())
      { this->storage_ = success<T>(res.unwrap()); }
    else
      { this->storage_ = failure<E>(res.unwrap_err()); }
    return *this;
  }

  /// @brief
  ///   move assignment operator for convertible basic_result
  template <mutability _mu, class U, class F>
    requires std::constructible_from<T, U>
          && std::constructible_from<E, F>
  constexpr basic_result& operator=(basic_result<_mu, U, F>&& res)
  {
    static_assert(is_mut_v<_mutability>, "Error: assignment to immutable result");
    if (res.is_ok())
      { this->storage_ = success<T>(res.unwrap()); }
    else
      { this->storage_ = failure<E>(res.unwrap_err()); }
    return *this;
  }

  /// @brief
  ///   copy assignment operator for convertible success
  template <class U> requires std::constructible_from<T, U>
  constexpr basic_result& operator=(success<U> const& _ok)
  {
    static_assert(is_mut_v<_mutability>, "Error: assignment to immutable result");
    this->storage_ = success<T>(_ok.get());
    return *this;
  }

  /// @brief
  ///   copy assignment operator for convertible failure
  template <class F> requires std::constructible_from<E, F>
  constexpr basic_result& operator=(failure<F> const& _err)
  {
    static_assert(is_mut_v<_mutability>, "Error: assignment to immutable result");
    this->storage_ = failure<E>(_err.get());
    return *this;
  }

  /// @brief
  ///   move assignment operator for convertible success
  template <class U> requires std::constructible_from<T, U>
  constexpr basic_result& operator=(success<U>&& _ok)
  {
    static_assert(is_mut_v<_mutability>, "Error: assignment to immutable result");
    this->storage_ = success<T>(std::move(_ok.get()));
    return *this;
  }

  /// @brief
  ///   move assignment operator for convertible failure
  template <class F> requires std::constructible_from<E, F>
  constexpr basic_result& operator=(failure<F>&& _err)
  {
    static_assert(is_mut_v<_mutability>, "Error: assignment to immutable result");
    this->storage_ = failure<E>(std::move(_err.get()));
    return *this;
  }

  /// @brief
  ///   explicit constructor for successful lvalue
  template <class U> requires std::constructible_from<T, const U&>
  constexpr explicit(!std::convertible_to<const U&, T>)
  basic_result(success<U> const& ok)
    : storage_{std::in_place_type<success<T>>, std::in_place, ok.get()}
  {}

  /// @brief
  ///   explicit constructor for successful rvalue
  template <class U> requires std::constructible_from<T, U&&>
  constexpr explicit(!std::convertible_to<U&&, T>)
  basic_result(success<U>&& ok)
    : storage_{std::in_place_type<success<T>>, std::in_place, static_cast<U&&>(ok.get())}
  {}

  /// @brief
  ///   non-explicit constructor for unsuccessful lvalue
  template <class F> requires std::constructible_from<E, F const&>
  constexpr explicit(!std::convertible_to<F const&, E>)
  basic_result(failure<F> const& err)
    : storage_{std::in_place_type<failure<E>>, std::in_place, err.get()}
  {}

  /// @brief
  ///   non-explicit constructor for unsuccessful rvalue
  template <class F> requires std::constructible_from<E, F&&>
  constexpr explicit(!std::convertible_to<F&&, E>)
  basic_result(failure<F>&& err)
    : storage_{std::in_place_type<failure<E>>, std::in_place, static_cast<F&&>(err.get())}
  {}

  constexpr explicit basic_result(success<>)
    : storage_{std::in_place_type<success<>>, std::monostate{}}
  {}

  constexpr explicit basic_result(failure<>)
    : storage_{std::in_place_type<failure<>>, std::monostate{}}
  {}

  /// @brief
  ///   in-place constructor for successful result
  template <class... Args>
      requires std::constructible_from<T, Args&&...>
  constexpr explicit basic_result(in_place_ok_t, Args && ... args)
    : storage_{std::in_place_type<success<T>>, std::in_place, std::forward<Args>(args)...}
  {}

  /// @brief
  ///   in-place constructor for unsuccessful result
  template <class... Args>
      requires std::constructible_from<E, Args&&...>
  constexpr explicit basic_result(in_place_err_t, Args && ... args)
    : storage_{std::in_place_type<failure<E>>, std::in_place, std::forward<Args>(args)...}
  {}

  /// @brief
  ///   in-place constructor with initializer_list for successful result
  template <class U, class... Args>
      requires std::constructible_from<T, std::initializer_list<U>, Args&&...>
  constexpr explicit basic_result(in_place_ok_t, std::initializer_list<U> il, Args && ... args)
    : storage_{std::in_place_type<success<T>>, std::in_place, il, std::forward<Args>(args)...}
  {}

  /// @brief
  ///   in-place constructor with initializer_list for unsuccessful result
  template <class U, class... Args>
      requires std::constructible_from<E, std::initializer_list<U>, Args&&...>
  constexpr explicit basic_result(in_place_err_t, std::initializer_list<U> il, Args && ... args)
    : storage_{std::in_place_type<failure<E>>, std::in_place, il, std::forward<Args>(args)...}
  {}

  /// @brief
  ///   Checks if self has a success value.
  ///
  /// @note
  ///   Returns true if the result is succsess.
  [[nodiscard("Warning: Do not discard the return value of `is_ok()`.")]]
  constexpr bool is_ok() const noexcept { return std::holds_alternative<success<T>>(storage_); }

  /// @brief
  ///   Checks if self has a failure value.
  ///
  /// @note
  ///   Returns true if the result is failure.
  [[nodiscard("Warning: Do not discard the return value of `is_err()`.")]]
  constexpr bool is_err() const noexcept { return std::holds_alternative<failure<E>>(storage_); }

  /// @brief
  ///   Converts from basic_result to bool.
  ///
  /// @note
  ///   Covert result to bool and returns true if the result is succsess.
  explicit constexpr operator bool() const noexcept { return std::holds_alternative<success<T>>(storage_); }

  /// @brief
  ///   Converts from basic_result to bool.
  ///
  /// @note
  ///   Covert result to bool and returns true if the result is failure.
  constexpr bool operator !() const noexcept { return std::holds_alternative<failure<E>>(storage_); }

  /// @brief
  ///   Converts from basic_result to `maybe<const T>`.
  ///
  /// @note
  ///   Converts self into a `maybe<const T>`, and discarding the failure, if any.
  constexpr
  maybe<std::remove_reference_t<ok_type>>
  ok() const& noexcept {
    if (is_ok()) {
      return maybe<std::remove_reference_t<ok_type>>(std::in_place, unwrap());
    }
    else {
      return nothing;
    }
  }

  /// @brief
  ///   Converts from basic_result to `maybe`.
  ///
  /// @note
  ///   Converts self into a `maybe<const E>`, and discarding the success, if any.
  constexpr
  maybe<std::remove_reference_t<err_type>>
  err() const& noexcept {
    if (is_err()) {
      return maybe<std::remove_reference_t<err_type>>(std::in_place, unwrap_err());
    }
    else {
      return nothing;
    }
  }

  /// @brief
  ///   Produces a new basic_result, containing a reference into the original, leaving the original in place.
  constexpr auto as_ref() const& noexcept
    -> basic_result<_mutability, std::remove_cvref_t<T> const&, std::remove_cvref_t<E> const&>
  {
    if ( is_ok() )
      return basic_result<_mutability, std::remove_cvref_t<T> const&, std::remove_cvref_t<E> const&>{in_place_ok, std::get<success<T>>(storage_).get()};
    else
      return basic_result<_mutability, std::remove_cvref_t<T> const&, std::remove_cvref_t<E> const&>{in_place_err, std::get<failure<E>>(storage_).get()};
  }

  /// @brief
  ///   Converts from `basic_result<mutability::mut, T, E>&` to `basic_result<mutability::immut, T&, E&>`.
  constexpr
  auto as_mut() & noexcept
    -> basic_result<mutability::immut, std::remove_reference_t<T>&, std::remove_reference_t<E>&>
  {
    static_assert(
      !std::is_const_v<std::remove_reference_t<T>>,
      "Error: ok_type is immutable");
    static_assert(
      !std::is_const_v<std::remove_reference_t<E>>,
      "Error: err_type is immutable");
    static_assert(
      is_mut_v<_mutability>,
      "Error: result is immutable");

    if ( is_ok() )
      return basic_result<mutability::immut, std::remove_reference_t<T>&, std::remove_reference_t<E>&>{in_place_ok, std::get<success<T>>(storage_).get()};
    else
      return basic_result<mutability::immut, std::remove_reference_t<T>&, std::remove_reference_t<E>&>{in_place_err, std::get<failure<E>>(storage_).get()};
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<U, E> by applying a function to a contained success value,
  ///   leaving an failure value untouched.
  ///
  /// @constrains
  ///   op: std::regular_invocable<T&&>
  ///
  /// @note
  ///   This function can be used to compose the results of two functions.
  constexpr auto map(std::regular_invocable<T&> auto&& op) &
    noexcept(std::is_nothrow_invocable_v<decltype(op), T&>)
  {
    using result_type = basic_result<_mutability, std::invoke_result_t<decltype(op), T>, E>;
    return is_ok()
               ? static_cast<result_type>(success{std::invoke(std::forward<decltype(op)>(op), std::get<success<T>>(storage_).get())})
               : static_cast<result_type>(failure{std::get<failure<E>>(storage_).get()});
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<U, E> by applying a function to a contained success value,
  ///   leaving an failure value untouched.
  ///
  /// @constrains
  ///   op: std::invocable<T&&>
  ///
  /// @note
  ///   This function can be used to compose the results of two functions.
  constexpr auto map(std::invocable<T const&> auto&& op) const&
    noexcept(std::is_nothrow_invocable_v<decltype(op), T const&>)
  {
    using result_type = basic_result<_mutability, std::invoke_result_t<decltype(op), T>, E>;
    return is_ok()
               ? static_cast<result_type>(success{std::invoke(std::forward<decltype(op)>(op), std::get<success<T>>(storage_).get())})
               : static_cast<result_type>(failure{std::get<failure<E>>(storage_).get()});
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<U, E> by applying a function to a contained success value,
  ///   leaving an failure value untouched.
  ///
  /// @constrains
  ///   op: std::invocable<T&&>
  ///
  /// @note
  ///   This function can be used to compose the results of two functions.
  constexpr auto map(std::invocable<T&&> auto&& op) &&
    noexcept(std::is_nothrow_invocable_v<decltype(op), T&&>)
  {
    using result_type = basic_result<_mutability, std::invoke_result_t<decltype(op), T>, E>;
    return is_ok()
               ? static_cast<result_type>(success{std::invoke(std::forward<decltype(op)>(op), std::move(std::get<success<T>>(storage_).get()))})
               : static_cast<result_type>(failure{std::move(std::get<failure<E>>(storage_).get())});
  }

  /// @brief
  ///   Maps a basic_result<T, E> to U by applying a function to a contained success value,
  ///   or a fallback function to a contained failure value.
  ///
  /// @constrains
  ///   _fallback: std::regular_invocable<E&>,
  ///   map: std::regular_invocable<T&>,
  ///   std::common_with<std::invoke_result_t<decltype(_fallback), E&>, std::invoke_result_t<decltype(_map), T&>>
  ///
  /// @note
  ///   This function can be used to unpack a successful result while handling an error.
  constexpr auto map_or_else(std::regular_invocable<E&> auto&& _fallback, std::regular_invocable<T&> auto&& _map) &
    noexcept(std::is_nothrow_invocable_v<decltype(_fallback), E&> && std::is_nothrow_invocable_v<decltype(_map), T&>)
    requires std::common_with<std::invoke_result_t<decltype(_fallback), E&>, std::invoke_result_t<decltype(_map), T&>>
  {
    using result_type = std::common_type_t<std::invoke_result_t<decltype(_map), T>, std::invoke_result_t<decltype(_fallback), E>>;
    return is_ok()
               ? static_cast<result_type>(std::invoke(std::forward<decltype(_map)>(_map), std::get<success<T>>(storage_).get()))
               : static_cast<result_type>(std::invoke(std::forward<decltype(_fallback)>(_fallback), std::get<failure<E>>(storage_).get()));
  }

  /// @brief
  ///   Maps a basic_result<T, E> to U by applying a function to a contained success value,
  ///   or a fallback function to a contained failure value.
  ///
  /// @constrains
  ///   _fallback: std::invocable<E&>,
  ///   map: std::invocable<T&>,
  ///   std::common_with<std::invoke_result_t<decltype(_fallback), E&>, std::invoke_result_t<decltype(_map), T&>>
  ///
  /// @note
  ///   This function can be used to unpack a successful result while handling an error.
  constexpr auto map_or_else(std::invocable<E const&> auto&& _fallback, std::invocable<T const&> auto&& _map) const&
    noexcept(std::is_nothrow_invocable_v<decltype(_fallback), E const&> && std::is_nothrow_invocable_v<decltype(_map), T const&>)
    requires std::common_with<std::invoke_result_t<decltype(_fallback), E const&>, std::invoke_result_t<decltype(_map), T const&>>
  {
    using result_type = std::common_type_t<std::invoke_result_t<decltype(_map), T>, std::invoke_result_t<decltype(_fallback), E>>;
    return is_ok()
               ? static_cast<result_type>(std::invoke(std::forward<decltype(_map)>(_map), std::get<success<T>>(storage_).get()))
               : static_cast<result_type>(std::invoke(std::forward<decltype(_fallback)>(_fallback), std::get<failure<E>>(storage_).get()));
  }

  /// @brief
  ///   Maps a basic_result<T, E> to U by applying a function to a contained success value,
  ///   or a fallback function to a contained failure value.
  ///
  /// @constrains
  ///   _fallback: std::invocable<E&>,
  ///   map: std::invocable<T&>,
  ///   std::common_with<std::invoke_result_t<decltype(_fallback), E&>, std::invoke_result_t<decltype(_map), T&>>
  ///
  /// @note
  ///   This function can be used to unpack a successful result while handling an error.
  constexpr auto map_or_else(std::invocable<E&&> auto&& _fallback, std::invocable<T&&> auto&& _map) &&
    noexcept(std::is_nothrow_invocable_v<decltype(_fallback), E&&> && std::is_nothrow_invocable_v<decltype(_map), T&&>)
    requires std::common_with<std::invoke_result_t<decltype(_fallback), E&&>, std::invoke_result_t<decltype(_map), T&&>>
  {
    using result_type = std::common_type_t<std::invoke_result_t<decltype(_map), T>, std::invoke_result_t<decltype(_fallback), E>>;
    return is_ok()
               ? static_cast<result_type>(std::invoke(std::forward<decltype(_map)>(_map), std::move(std::get<success<T>>(storage_).get())))
               : static_cast<result_type>(std::invoke(std::forward<decltype(_fallback)>(_fallback), std::move(std::get<failure<E>>(storage_).get())));
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<T, F> by applying a function to a contained failure value,
  ///   leaving an success value untouched.
  ///
  /// @constrains
  ///   op: std::regular_invocable<E&>
  ///
  /// @note
  ///   This function can be used to pass through a successful result while handling an error.
  constexpr auto map_err(std::regular_invocable<E&> auto&& op) &
    noexcept(std::is_nothrow_invocable_v<decltype(op), E>)
  {
    using result_type = basic_result<_mutability, T, std::invoke_result_t<decltype(op), E>>;
    return is_err()
               ? static_cast<result_type>(failure{std::invoke(std::forward<decltype(op)>(op), std::get<failure<E>>(storage_).get())})
               : static_cast<result_type>(success{std::get<success<T>>(storage_).get()});
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<T, F> by applying a function to a contained failure value,
  ///   leaving an success value untouched.
  ///
  /// @constrains
  ///   op: std::invocable<E const&>
  ///
  /// @note
  ///   This function can be used to pass through a successful result while handling an error.
  constexpr auto map_err(std::invocable<E const&> auto&& op) const&
    noexcept(std::is_nothrow_invocable_v<decltype(op), E>)
  {
    using result_type = basic_result<_mutability, T, std::invoke_result_t<decltype(op), E>>;
    return is_err()
               ? static_cast<result_type>(failure{std::invoke(std::forward<decltype(op)>(op), std::get<failure<E>>(storage_).get())})
               : static_cast<result_type>(success{std::get<success<T>>(storage_).get()});
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<T, F> by applying a function to a contained failure value,
  ///   leaving an success value untouched.
  ///
  /// @constrains
  ///   { std::invoke(op, unwrap_err()) }
  ///
  /// @note
  ///   This function can be used to pass through a successful result while handling an error.
  constexpr auto map_err(std::invocable<E&&> auto&& op) &&
    noexcept(std::is_nothrow_invocable_v<decltype(op), E>)
  {
    using result_type = basic_result<_mutability, T, std::invoke_result_t<decltype(op), E>>;
    return is_err()
               ? static_cast<result_type>(failure{std::invoke(std::forward<decltype(op)>(op), std::move(std::get<failure<E>>(storage_).get()))})
               : static_cast<result_type>(success{std::move(std::get<success<T>>(storage_).get())});
  }

  /// @brief
  ///   Calls `op` if the result is success, otherwise; returns the failure value of self.
  ///
  /// @constrains
  ///   op: std::regular_invocable<T&>
  ///
  /// @note
  ///   This function can be used for control flow based on result values.
  constexpr auto and_then(std::regular_invocable<T&> auto&& op) &
    noexcept(std::is_nothrow_invocable_v<decltype(op), T>)
  {
    using result_type = std::invoke_result_t<decltype(op), T>;
    return is_ok()
               ? std::invoke(std::forward<decltype(op)>(op), std::get<success<T>>(storage_).get())
               : static_cast<result_type>(failure{std::get<failure<E>>(storage_).get()});
  }

  /// @brief
  ///   Calls `op` if the result is success, otherwise; returns the failure value of self.
  ///
  /// @constrains
  ///   op: std::invocable<T const&>
  ///
  /// @note
  ///   This function can be used for control flow based on result values.
  constexpr auto and_then(std::invocable<T const&> auto&& op) const &
    noexcept(std::is_nothrow_invocable_v<decltype(op), T>)
  {
    using result_type = std::invoke_result_t<decltype(op), T>;
    return is_ok()
               ? std::invoke(std::forward<decltype(op)>(op), std::get<success<T>>(storage_).get())
               : static_cast<result_type>(failure{std::get<failure<E>>(storage_).get()});
  }

  /// @brief
  ///   Calls `op` if the result is success, otherwise; returns the failure value of self.
  ///
  /// @constrains
  ///   op: std::invocable<T&&>
  ///
  /// @note
  ///   This function can be used for control flow based on result values.
  constexpr auto and_then(std::invocable<T&&> auto&& op) &&
    noexcept(std::is_nothrow_invocable_v<decltype(op), T>)
  {
    using result_type = std::invoke_result_t<decltype(op), T>;
    return is_ok()
               ? std::invoke(std::forward<decltype(op)>(op), std::move(std::get<success<T>>(storage_).get()))
               : static_cast<result_type>(failure{std::move(std::get<failure<E>>(storage_).get())});
  }

  /// @brief
  ///   Calls `op` if the result is failure, otherwise; returns the success value of self.
  ///
  /// @constrains
  ///   op: std::regular_invocable<E&>
  ///
  /// @note
  ///   This function can be used for control flow based on result values.
  constexpr auto or_else(std::regular_invocable<E&> auto&& op) &
    noexcept(std::is_nothrow_invocable_v<decltype(op), E>)
  {
    using result_type = std::invoke_result_t<decltype(op), E>;
    return is_err()
               ? std::invoke(std::forward<decltype(op)>(op), std::get<failure<E>>(storage_).get())
               : static_cast<result_type>(success{std::get<success<T>>(storage_).get()});
  }

  /// @brief
  ///   Calls `op` if the result is failure, otherwise; returns the success value of self.
  ///
  /// @constrains
  ///   op: std::invocable<E const&>
  ///
  /// @note
  ///   This function can be used for control flow based on result values.
  constexpr auto or_else(std::invocable<E const&> auto&& op) const&
    noexcept(std::is_nothrow_invocable_v<decltype(op), E>)
  {
    using result_type = std::invoke_result_t<decltype(op), E>;
    return is_err()
               ? std::invoke(std::forward<decltype(op)>(op), std::get<failure<E>>(storage_).get())
               : static_cast<result_type>(success{std::get<success<T>>(storage_).get()});
  }

  /// @brief
  ///   Calls `op` if the result is failure, otherwise; returns the success value of self.
  ///
  /// @constrains
  ///   op: std::invocable<E&&>
  ///
  /// @note
  ///   This function can be used for control flow based on result values.
  constexpr auto or_else(std::invocable<E&&> auto&& op) &&
    noexcept(std::is_nothrow_invocable_v<decltype(op), E>)
  {
    using result_type = std::invoke_result_t<decltype(op), E>;
    return is_err()
               ? std::invoke(std::forward<decltype(op)>(op), std::get<failure<E>>(std::move(storage_)).get())
               : static_cast<result_type>(success{std::get<success<T>>(std::move(storage_)).get()});
  }

  /// @brief
  ///   Returns `res` if the result is success, otherwise; returns the failure value of self.
  template <mutability _mu, class U>
  constexpr decltype(auto) conj(basic_result<_mu, U, E> const& res) const& noexcept
  {
    using result_type = basic_result<_mutability && _mu, U, E>;
    return this->is_err()
               ? static_cast<result_type>(failure{std::get<failure<E>>(storage_).get()})
               : res.is_err() ? static_cast<result_type>(failure{res.unwrap_err()})
                              : static_cast<result_type>(success{res.unwrap()});
  }

  /// @brief
  ///   Returns `res` if the result is success, otherwise; returns the failure value of self.
  template <mutability _mu, class U>
  constexpr decltype(auto) operator&&(basic_result<_mu, U, E> const& res) const& noexcept
  {
    return this->conj(res);
  }

  /// @brief
  ///   Returns res if the result is failure, otherwise returns the success value of self.
  ///
  /// @note
  ///   Arguments passed to or are eagerly evaluated;
  ///   if you are passing the result of a function call,
  ///   it is recommended to use `or_else`,
  ///   which is lazily evaluated.
  template <mutability _mut, class F>
  constexpr decltype(auto) disj(basic_result<_mut, T, F> const& res) const& noexcept
  {
    using result_type = basic_result<_mutability, T, F>;
    return this->is_ok()
               ? static_cast<result_type>(success{std::get<success<T>>(storage_).get()})
               : res.is_ok() ? static_cast<result_type>(success{res.unwrap()})
                             : static_cast<result_type>(failure{res.unwrap_err()});
  }

  /// @brief
  ///   Returns res if the result is failure, otherwise returns the success value of self.
  ///
  /// @note
  ///   Arguments passed to or are eagerly evaluated;
  ///   if you are passing the result of a function call,
  ///   it is recommended to use `or_else`,
  ///   which is lazily evaluated.
  template <mutability _mut, class F>
  constexpr decltype(auto) operator||(basic_result<_mut, T, F> const& res) const& noexcept
  {
    return this->disj(res);
  }

  /// @brief
  ///   Converts from basic_result<T, E> &` to basic_result< <T as Deref>::Target&, E&>.
  ///
  /// @constrains
  ///   T: dereferencable
  ///
  /// @note
  ///   Leaves the original basic_result in-place,
  ///   creating a new one with a reference to the original one,
  ///   additionally coercing the success arm of the basic_result via `operator*`.
  constexpr auto indirect_ok() & requires (dereferencable<T>) {
    using indirect_ok_result = basic_result<_mutability, std::remove_reference_t<deref_type_t<T>>&, std::remove_reference_t<E>&>;
    if ( is_ok() ) {
      return indirect_ok_result{in_place_ok, *unwrap()};
    }
    else {
      return indirect_ok_result{in_place_err, unwrap_err()};
    }
  }

  /// @brief
  ///   Converts from basic_result<T, E> &` to basic_result<T::Target const&, E const&>.
  ///
  /// @constrains
  ///   T: dereferencable
  ///
  /// @note
  ///   Leaves the original basic_result in-place,
  ///   creating a new one with a reference to the original one,
  ///   additionally coercing the success arm of the basic_result via `operator*`.
  constexpr auto indirect_ok() const& requires (dereferencable<T>) {
    using const_indirect_ok_result = basic_result<_mutability, std::remove_cvref_t<deref_type_t<T>> const&, std::remove_cvref_t<E> const&>;
    if ( is_ok() ) {
      return const_indirect_ok_result{in_place_ok, *unwrap()};
    }
    else {
      return const_indirect_ok_result{in_place_err, unwrap_err()};
    }
  }

  /// @brief
  ///   Converts from basic_result<T, E> &` to basic_result<dangling<T::Target&>, dangling<E&>>.
  ///
  /// @constrains
  ///   T: dereferencable
  ///
  /// @note
  ///   Leaves the original basic_result in-place,
  ///   creating a new one with a reference to the original one,
  ///   additionally coercing the success arm of the basic_result via `operator*`.
  ///
  /// @warning
  ///   Contained reference may be exhausted because of original result is rvalue.
  constexpr auto indirect_ok() && requires (dereferencable<T>) {
    using dangling_indirect_ok_result = basic_result<_mutability, dangling<std::reference_wrapper<std::remove_reference_t<deref_type_t<T>>>>, dangling<std::reference_wrapper<std::remove_reference_t<E>>>>;
    if ( is_ok() ) {
      return dangling_indirect_ok_result{in_place_ok, std::ref(*unwrap())};
    }
    else {
      return dangling_indirect_ok_result{in_place_err, unwrap_err()};
    }
  }

  constexpr void indirect_ok() const&& = delete;

  /// @brief
  ///   Converts from basic_result<T, E> &` to basic_result<T&, E::Target&>.
  ///
  /// @constrains
  ///   (E e) { *e }
  ///
  /// @note
  ///   Leaves the original basic_result in-place,
  ///   creating a new one with a reference to the original one,
  ///   additionally coercing the failure arm of the basic_result via `operator*`.
  constexpr auto indirect_err() & requires (dereferencable<E>) {
    using indirect_err_result = basic_result<_mutability, std::remove_reference_t<T>&, std::remove_reference_t<deref_type_t<E>>&>;
    if ( is_ok() ) {
      return indirect_err_result{in_place_ok, unwrap()};
    }
    else {
      return indirect_err_result{in_place_err, *unwrap_err()};
    }
  }

  /// @brief
  ///   Converts from basic_result<T, E> &` to basic_result<T const&, E::Target const&>.
  ///
  /// @constrains
  ///   (E e) { *e }
  ///
  /// @note
  ///   Leaves the original basic_result in-place,
  ///   creating a new one with a reference to the original one,
  ///   additionally coercing the failure arm of the basic_result via `operator*`.
  constexpr auto indirect_err() const& requires (dereferencable<E>) {
    using const_indirect_err_result = basic_result<_mutability, std::remove_cvref_t<T> const&, std::remove_cvref_t<deref_type_t<E>> const&>;
    if ( is_ok() ) {
      return const_indirect_err_result{in_place_ok, unwrap()};
    }
    else {
      return const_indirect_err_result{in_place_err, *unwrap_err()};
    }
  }

  /// @brief
  ///   Converts from basic_result<T, E> &` to basic_result<dangling<T&>, dangling<E::Target&>>.
  ///
  /// @constrains
  ///   (E e) { *e }
  ///
  /// @note
  ///   Leaves the original basic_result in-place,
  ///   creating a new one with a reference to the original one,
  ///   additionally coercing the failure arm of the basic_result via `operator*`.
  ///
  /// @warning
  ///   Contained reference may be exhausted because of original result is rvalue.
  constexpr auto indirect_err() && requires (dereferencable<E>) {
    using dangling_indirect_err_result = basic_result<_mutability, dangling<std::remove_reference_t<T>&>, dangling<std::reference_wrapper<std::remove_reference_t<deref_type_t<E>>>>>;
    if ( is_ok() ) {
      return dangling_indirect_err_result{in_place_ok, unwrap()};
    }
    else {
      return dangling_indirect_err_result{in_place_err, std::ref(*unwrap_err())};
    }
  }

  constexpr void indirect_err() const&& = delete;

  /// @brief
  ///   Converts from basic_result<T, E> &` to basic_result<T::Target&, E::Target&>.
  ///
  /// @constrains
  ///   (T t) { *t };
  ///   (E e) { *e }
  ///
  /// @note
  ///   Leaves the original basic_result in-place,
  ///   creating a new one with a reference to the original one,
  ///   additionally coercing the success and failure arm of the basic_result via `operator*`.
  constexpr auto indirect() & requires (dereferencable<E> && dereferencable<E>) {
    using indirect_result = basic_result<_mutability, std::remove_reference_t<deref_type_t<T>>&, std::remove_reference_t<deref_type_t<E>>&>;
    if ( is_ok() ) {
      return indirect_result{in_place_ok, *unwrap()};
    }
    else {
      return indirect_result{in_place_err, *unwrap_err()};
    }
  }

  /// @brief
  ///   Converts from basic_result<T, E> &` to basic_result<T::Target const&, E::Target const&>.
  ///
  /// @constrains
  ///   (T t) { *t };
  ///   (E e) { *e }
  ///
  /// @note
  ///   Leaves the original basic_result in-place,
  ///   creating a new one with a reference to the original one,
  ///   additionally coercing the success and failure arm of the basic_result via `operator*`.
  constexpr auto indirect() const& requires (dereferencable<E> && dereferencable<E>) {
    using const_indirect_result = basic_result<_mutability, std::remove_cvref_t<deref_type_t<T>> const&, std::remove_cvref_t<deref_type_t<E>> const&>;
    if ( is_ok() ) {
      return const_indirect_result{in_place_ok, *unwrap()};
    }
    else {
      return const_indirect_result{in_place_err, *unwrap_err()};
    }
  }

  /// @brief
  ///   Converts from basic_result<T, E> &` to basic_result<dangling<T::Target&>, dangling<E::Target&>>.
  ///
  /// @constrains
  ///   (T t) { *t };
  ///   (E e) { *e }
  ///
  /// @note
  ///   Leaves the original basic_result in-place,
  ///   creating a new one with a reference to the original one,
  ///   additionally coercing the success and failure arm of the basic_result via `operator*`.
  ///
  /// @warning
  ///   Contained reference may be exhausted because of original result is rvalue.
  constexpr auto indirect() && requires (dereferencable<E> && dereferencable<E>) {
    using dangling_indirect_result = basic_result<_mutability, dangling<std::reference_wrapper<std::remove_reference_t<deref_type_t<T>>>>, dangling<std::reference_wrapper<std::remove_reference_t<deref_type_t<E>>>>>;
    if ( is_ok() ) {
      return dangling_indirect_result{in_place_ok, std::ref(*unwrap())};
    }
    else {
      return dangling_indirect_result{in_place_err, std::ref(*unwrap_err())};
    }
  }

  constexpr void indirect() const&& = delete;

  /// @brief
  ///   Returns the contained value or a default.
  ///
  /// @constraints
  ///   T: requires (is_maybe<T>::value)
  ///
  /// @note
  ///   Consumes the self argument then,
  ///   if success, returns the contained value,
  ///   otherwise; if failure, returns the default value for that type.
  constexpr auto
  transpose() const& requires (is_maybe<T>::value)
  {
    using return_type = maybe<basic_result<_mutability ,typename std::remove_cvref_t<T>::value_type, E>>;
    if (is_ok()) {
      if (auto const& may = unwrap()) {
        return return_type{std::in_place, in_place_ok, may.unwrap()};
      }
      else {
        return return_type{mitama::nothing};
      }
    }
    else {
      return return_type{std::in_place, in_place_err, unwrap_err()};
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///   Else, it returns optb.
  ///
  /// @constrains
  ///   { is_ok() ? unwrap() : optb }
  ///
  /// @note
  ///   Arguments passed to `unwrap_or` are eagerly evaluated;
  ///   if you are passing the result of a function call,
  ///   it is recommended to use `unwrap_or_else`,
  ///   which is lazily evaluated.
  decltype(auto) unwrap_or(std::common_with<T&> auto&& optb) & noexcept
  {
    return is_ok() ? std::get<success<T>>(storage_).get()
                   : std::forward<decltype(optb)>(optb);
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///   Else, it returns optb.
  ///
  /// @constrains
  ///   { is_ok() ? unwrap() : optb }
  ///
  /// @note
  ///   Arguments passed to `unwrap_or` are eagerly evaluated;
  ///   if you are passing the result of a function call,
  ///   it is recommended to use `unwrap_or_else`,
  ///   which is lazily evaluated.
  decltype(auto) unwrap_or(std::common_with<T const&> auto&& optb) const& noexcept
  {
    return is_ok() ? std::get<success<T>>(storage_).get()
                   : std::forward<decltype(optb)>(optb);
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///   Else, it returns optb.
  ///
  /// @constrains
  ///   { is_ok() ? unwrap() : optb }
  ///
  /// @note
  ///   Arguments passed to `unwrap_or` are eagerly evaluated;
  ///   if you are passing the result of a function call,
  ///   it is recommended to use `unwrap_or_else`,
  ///   which is lazily evaluated.
  decltype(auto) unwrap_or(std::common_with<std::remove_reference_t<T>&&> auto&& optb) && noexcept
  {
    return is_ok() ? std::move(std::get<success<T>>(storage_).get())
                   : std::forward<decltype(optb)>(optb);
  }

  // Since C++20, enable initialize aggregates from a parenthesized list of values;
  // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0960r3.html
  /// @brief
  ///   Returns the contained value or a default.
  ///
  /// @note
  ///   Consumes the self argument then,
  ///   if success, returns the contained value,
  ///   otherwise; if Err, returns the default value for that type.
  T unwrap_or_default() const requires (std::default_initializable<T>)
  {
    return is_ok() ? unwrap() : T();
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @constrains
  ///   op: requires std::is_invocable_r_v<T, O, E>
  template <class O>
  auto unwrap_or_else(O && op) const
    noexcept(std::is_nothrow_invocable_r_v<T, O, E>)
    requires std::is_invocable_r_v<T, O, E>
  {
    return is_ok() ? std::get<success<T>>(storage_).get() : std::invoke(std::forward<O>(op), std::get<failure<E>>(storage_).get());
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @constrains
  ///   op: requires std::is_invocable_r_v<T, O>
  template <class O>
  auto unwrap_or_else(O && op) const
    noexcept(std::is_nothrow_invocable_r_v<T, O>)
    requires std::is_invocable_r_v<T, O> && (!std::is_invocable_r_v<T, O, E>)
  {
    return is_ok() ? std::get<success<T>>(storage_).get() : std::invoke(std::forward<O>(op));
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @panics
  ///   Panics if the value is an failure, with a panic message provided by the failure's value.
  force_add_const_t<T>&
  unwrap() const& {
    if constexpr (display<E>) {
      if ( is_ok() ) {
        return std::get<success<T>>(storage_).get();
      }
      else {
        PANIC("called `basic_result::unwrap()` on a value: `{}`", std::get<failure<E>>(storage_));
      }      
    }
    else {
      if ( is_ok() ) {
        return std::get<success<T>>(storage_).get();
      }
      else {
        PANIC("called `basic_result::unwrap()` on a value `failure(?)`");
      }
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @panics
  ///   Panics if the value is an failure, with a panic message provided by the failure's value.
  std::conditional_t<is_mut_v<_mutability>, T&, force_add_const_t<T>&>
  unwrap() & {
    if constexpr (display<E>) {
      if ( is_ok() ) {
        return std::get<success<T>>(storage_).get();
      }
      else {
        PANIC("called `basic_result::unwrap()` on a value: `{}`", std::get<failure<E>>(storage_));
      }      
    }
    else {
      if ( is_ok() ) {
        return std::get<success<T>>(storage_).get();
      }
      else {
        PANIC("called `basic_result::unwrap()` on a value `failure(?)`");
      }
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an failure.
  ///
  /// @panics
  ///   Panics if the value is an success, with a panic message provided by the success's value.
  force_add_const_t<E>&
  unwrap_err() const& {
    if constexpr (display<T>) {
      if ( is_err() ) {
        return std::get<failure<E>>(storage_).get();
      }
      else {
        PANIC("called `basic_result::unwrap_err()` on a value: `{}`", std::get<success<T>>(storage_));
      }
    }
    else {
      if ( is_err() ) {
        return std::get<failure<E>>(storage_).get();
      }
      else {
        PANIC("called `basic_result::unwrap_err()` on a value `success(?)`");
      }
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an failure.
  ///
  /// @panics
  ///   Panics if the value is an success, with a panic message provided by the success's value.
  std::conditional_t<is_mut_v<_mutability>, E&, force_add_const_t<E>&>
  unwrap_err() & {
    if constexpr (display<T>) {
      if ( is_err() ) {
        return std::get<failure<E>>(storage_).get();
      }
      else {
        PANIC("called `basic_result::unwrap_err()` on a value: `{}`", std::get<success<T>>(storage_));
      }
    }
    else {
      if ( is_err() ) {
        return std::get<failure<E>>(storage_).get();
      }
      else {
        PANIC("called `basic_result::unwrap_err()` on a value `success(?)`)");
      }
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @panics
  ///   Panics if the value is an failure, with a panic message including the passed message, and the content of the failure.
  force_add_const_t<T>&
  expect(std::string_view msg) const& {
    if ( is_err() )
      PANIC("{}: {}", msg, unwrap_err());
    else
      return unwrap();
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @panics
  ///   Panics if the value is an failure, with a panic message including the passed message, and the content of the failure.
  decltype(auto)
  expect(std::string_view msg) & {
    if ( is_err() )
      PANIC("{}: {}", msg, unwrap_err());
    else
      return unwrap();
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an failure.
  ///
  /// @panics
  ///   Panics if the value is an success, with a panic message including the passed message, and the content of the success.
  force_add_const_t<E>&
  expect_err(std::string_view msg) const& {
    if ( is_ok() )
      PANIC("{}: {}", msg, unwrap());
    else
      return unwrap_err();
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an failure.
  ///
  /// @panics
  ///   Panics if the value is an success, with a panic message including the passed message, and the content of the success.
  decltype(auto)
  expect_err(std::string_view msg) & {
    if ( is_ok() )
      PANIC("{}: {}", msg, unwrap());
    else
      return unwrap_err();
  }

  template <class F>
  constexpr
  std::enable_if_t<std::is_invocable_v<F&&, T>>
  and_finally(F&& f) const& {
    if (this->is_ok())
      std::invoke(std::forward<F>(f), unwrap());
  }

  template <class F>
  constexpr
  std::enable_if_t<std::is_invocable_v<F&&, E>>
  or_finally(F&& f) const& {
    if (this->is_err())
      std::invoke(std::forward<F>(f), unwrap_err());
  }

  template <class F>
  constexpr
  std::enable_if_t<
    std::disjunction_v<
      std::is_invocable<F, T&>,
      std::is_invocable<F>>,
  basic_result&>
  and_peek(F&& f) &
  {
    if constexpr (std::is_invocable_v<F, T&>) {
      if (is_ok())
        std::invoke(std::forward<F>(f), unwrap());
    }
    else {
      if (is_ok())
        std::invoke(std::forward<F>(f));
    }
    return *this;
  }

  template <class F>
  std::enable_if_t<
    std::disjunction_v<
      std::is_invocable<F, T const&>,
      std::is_invocable<F>>,
  basic_result const&>
  and_peek(F&& f) const&
  {
    if constexpr (std::is_invocable_v<F, T const&>) {
      if (is_ok())
        std::invoke(std::forward<F>(f), unwrap());
    }
    else {
      if (is_ok())
        std::invoke(std::forward<F>(f));
    }
    return *this;
  }

  template <class F>
  std::enable_if_t<
    std::disjunction_v<
      std::is_invocable<F, T&&>,
      std::is_invocable<F>>,
  basic_result&&>
  and_peek(F&& f) &&
  {
    if constexpr (std::is_invocable_v<F, T&&>) {
      if (is_ok())
        std::invoke(std::forward<F>(f), unwrap());
    }
    else {
      if (is_ok())
        std::invoke(std::forward<F>(f));
    }
    return std::move(*this);
  }

  template <class F>
  std::enable_if_t<
    std::disjunction_v<
      std::is_invocable<F, E&>,
      std::is_invocable<F>>,
  basic_result&>
  or_peek(F&& f) & {
    if constexpr (std::is_invocable_v<F, E&>) {
      if (is_err())
        std::invoke(std::forward<F>(f), unwrap_err());
    }
    else {
      if (is_err())
        std::invoke(std::forward<F>(f));
    }
    return *this;
  }

  template <class F>
  std::enable_if_t<
    std::disjunction_v<
      std::is_invocable<F, E const&>,
      std::is_invocable<F>>,
  basic_result const&>
  or_peek(F&& f) const&
  {
    if constexpr (std::is_invocable_v<F, E const&>) {
      if (is_err())
        std::invoke(std::forward<F>(f), unwrap_err());
    }
    else {
      if (is_err())
        std::invoke(std::forward<F>(f));
    }
    return *this;
  }

  template <class F>
  std::enable_if_t<
    std::disjunction_v<
      std::is_invocable<F, E&&>,
      std::is_invocable<F>>,
  basic_result&&>
  or_peek(F&& f) &&
  {
    if constexpr (std::is_invocable_v<F, E&&>) {
      if (is_err())
        std::invoke(std::forward<F>(f), unwrap_err());
    }
    else {
      if (is_err())
        std::invoke(std::forward<F>(f));
    }
    return std::move(*this);
  }

  template <class F>
  auto map_anything_else(F&& f) &
    noexcept(std::is_nothrow_invocable_v<F, E&> && std::is_nothrow_invocable_v<F, T&>)
    requires std::regular_invocable<F&&, T&>
          && std::regular_invocable<F&&, E&>
          && std::common_with<std::invoke_result_t<F&&, T&>, std::invoke_result_t<F&&, E&>>
  {
    auto decay_copy = [](auto&& some) -> std::remove_const_t<std::remove_reference_t<decltype(some)>> { return std::forward<decltype(some)>(some); };
    return this->map_or_else(decay_copy(std::forward<F>(f)), decay_copy(std::forward<F>(f)));
  }

  template <class F>
  auto map_anything_else(F&& f) const&
    noexcept(std::is_nothrow_invocable_v<F, E const&> && std::is_nothrow_invocable_v<F, T const&>)
    requires std::regular_invocable<F&&, T const&>
          && std::regular_invocable<F&&, E const&>
          && std::common_with<std::invoke_result_t<F&&, T const&>, std::invoke_result_t<F&&, E const&>>
  {
    auto decay_copy = [](auto&& some) -> std::remove_const_t<std::remove_reference_t<decltype(some)>> { return std::forward<decltype(some)>(some); };
    return this->map_or_else(decay_copy(std::forward<F>(f)), decay_copy(std::forward<F>(f)));
  }

  template <class F>
  auto map_anything_else(F&& f) const&
    noexcept(std::is_nothrow_invocable_v<F, std::remove_reference_t<E>&&> && std::is_nothrow_invocable_v<F, std::remove_reference_t<T>&&>)
    requires std::regular_invocable<F&&, std::remove_reference_t<T>&&>
          && std::regular_invocable<F&&, std::remove_reference_t<E>&&>
          && std::common_with<std::invoke_result_t<F&&, std::remove_reference_t<T>&&>, std::invoke_result_t<F&&, std::remove_reference_t<E>&&>>
  {
    auto decay_copy = [](auto&& some) -> std::remove_const_t<std::remove_reference_t<decltype(some)>> { return std::forward<decltype(some)>(some); };
    return std::move(*this).map_or_else(decay_copy(std::forward<F>(f)), decay_copy(std::forward<F>(f)));
  }

  template <mutability _, std::totally_ordered_with<T> U, std::totally_ordered_with<E> F>
  constexpr std::strong_ordering operator<=>(basic_result<_, U, F> const& other) const {
    if (this->is_ok() && other.is_err()) return std::strong_ordering::greater;
    else if (this->is_err() && other.is_ok()) return std::strong_ordering::less;
    else if (this->is_ok() && other.is_ok()) {
      if (this->unwrap() < other.unwrap()) return std::strong_ordering::less;
      if (this->unwrap() > other.unwrap()) return std::strong_ordering::greater;
      else return std::strong_ordering::equivalent;
    }
    else {
      if (this->unwrap_err() < other.unwrap_err()) return std::strong_ordering::less;
      if (this->unwrap_err() > other.unwrap_err()) return std::strong_ordering::greater;
      else return std::strong_ordering::equivalent;
    }
  }

  template <std::totally_ordered_with<T> U>
  constexpr std::strong_ordering operator<=>(success<U> const& other) const {
    if (this->is_err()) return std::strong_ordering::less;
    else {
      if (this->unwrap() < other.get()) return std::strong_ordering::less;
      if (this->unwrap() > other.get()) return std::strong_ordering::greater;
      else return std::strong_ordering::equivalent;
    }
  }

  template <std::totally_ordered_with<E> F>
  constexpr std::strong_ordering operator<=>(failure<F> const& other) const {
    if (this->is_ok()) return std::strong_ordering::greater;
    else {
      if (this->unwrap_err() < other.get()) return std::strong_ordering::less;
      if (this->unwrap_err() > other.get()) return std::strong_ordering::greater;
      else return std::strong_ordering::equivalent;
    }
  }

  constexpr std::strong_ordering operator<=>(ok_type const& other) const
  { return *this <=> success(other); }
};

} // namespace mitama

#endif
