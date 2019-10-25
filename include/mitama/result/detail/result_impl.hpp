#ifndef MITAMA_RESULT_IMPL_HPP
#define MITAMA_RESULT_IMPL_HPP

#include <mitama/result/detail/meta.hpp>
#include <mitama/result/detail/fwd.hpp>
#include <mitama/result/traits/impl_traits.hpp>
#include <mitama/result/traits/Deref.hpp>
#include <mitama/result/detail/dangling.hpp>
#include <mitama/maybe/maybe.hpp>
#include <optional>
#include <functional>

namespace mitama {

template <class, class = void>
class unwrap_or_default_friend_injector
{
public:
  void unwrap_or_default() const = delete;
};

/// @impl
///   impl<_mutability, T, E> basic_result<_mutability, T, E>
///   where
///     T: Default
template <mutability _mu, class T, class E>
class unwrap_or_default_friend_injector<basic_result<_mu, T, E>,
                                        std::enable_if_t<std::disjunction_v<std::is_default_constructible<T>, std::is_aggregate<T>>>>
{
public:
  /// @brief
  ///   Returns the contained value or a default.
  ///
  /// @note
  ///   Consumes the self argument then,
  ///   if success, returns the contained value,
  ///   otherwise; if Err, returns the default value for that type.
  T unwrap_or_default() const
  {
    if constexpr (std::is_aggregate_v<T>){
      return static_cast<basic_result<_mu, T, E> const *>(this)->is_ok()
        ? static_cast<basic_result<_mu, T, E> const *>(this)->unwrap()
        : T{};
    }
    else {
      return static_cast<basic_result<_mu, T, E> const *>(this)->is_ok()
        ? static_cast<basic_result<_mu, T, E> const *>(this)->unwrap()
        : T();
    }
  }
};

template <class>
class transpose_friend_injector
{
public:
  void transpose() const = delete;
};

/// @impl
///   impl<_mutability, T, E> basic_result<_mutability, Option<T>, E>
template <mutability _mutability, class T, class E>
class transpose_friend_injector<basic_result<_mutability, maybe<T>, E>>
{
public:
  /// @brief
  ///   Returns the contained value or a default.
  ///
  /// @note
  ///   Consumes the self argument then,
  ///   if success, returns the contained value,
  ///   otherwise; if failure, returns the default value for that type.
  maybe<basic_result<_mutability ,T, E>> transpose() const &
  {
    if (static_cast<basic_result<_mutability, maybe<T>, E>const*>(this)->is_ok()) {
      if (auto const& may = static_cast<basic_result<_mutability, maybe<T>, E>const*>(this)->unwrap()) {
        return maybe<basic_result<_mutability ,T, E>>{std::in_place, in_place_ok, may.unwrap()};
      }
      else {
        return mitama::nothing;
      }
    }
    else {
        return maybe<basic_result<_mutability ,T, E>>{std::in_place, in_place_err, static_cast<basic_result<_mutability, maybe<T>, E>const*>(this)->unwrap_err()};
    }
  }
};

template <class, class = void>
class indirect_friend_injector
{
public:
  void indirect() const = delete;
  void indirect_ok() const = delete;
  void indirect_err() const = delete;
};

/// @impl
///   impl<_mutability, T, E> basic_result<_mutability, T, E>
///   where
///     T: Deref
///     E: Deref
template <mutability _mutability, class T, class E>
class indirect_friend_injector<basic_result<_mutability, T, E>, 
                            std::enable_if_t<
                              std::conjunction_v<
                                traits::is_dereferencable<T>,
                                traits::is_dereferencable<E>
                            >>>
{
  using indirect_ok_result = basic_result<_mutability, std::remove_reference_t<typename traits::Deref<T>::Target>&, std::remove_reference_t<E>&>;
  using indirect_err_result = basic_result<_mutability, std::remove_reference_t<T>&, std::remove_reference_t<typename traits::Deref<E>::Target>&>;
  using indirect_result = basic_result<_mutability, std::remove_reference_t<typename traits::Deref<T>::Target>&, std::remove_reference_t<typename traits::Deref<E>::Target>&>;
  using const_indirect_ok_result = basic_result<_mutability, meta::remove_cvr_t<typename traits::Deref<T>::Target> const&, meta::remove_cvr_t<E> const&>;
  using const_indirect_err_result = basic_result<_mutability, meta::remove_cvr_t<T> const&, meta::remove_cvr_t<typename traits::Deref<E>::Target> const&>;
  using const_indirect_result = basic_result<_mutability, meta::remove_cvr_t<typename traits::Deref<T>::Target> const&, meta::remove_cvr_t<typename traits::Deref<E>::Target> const&>;
  using dangling_indirect_ok_result = basic_result<_mutability, dangling<std::reference_wrapper<std::remove_reference_t<typename traits::Deref<T>::Target>>>, dangling<std::reference_wrapper<std::remove_reference_t<E>>>>;
  using dangling_indirect_err_result = basic_result<_mutability, dangling<std::remove_reference_t<T>&>, dangling<std::reference_wrapper<std::remove_reference_t<typename traits::Deref<E>::Target>>>>;
  using dangling_indirect_result = basic_result<_mutability, dangling<std::reference_wrapper<std::remove_reference_t<typename traits::Deref<T>::Target>>>, dangling<std::reference_wrapper<std::remove_reference_t<typename traits::Deref<E>::Target>>>>;
public:
  /// @brief
  ///   Converts from basic_result<T, E> &` to basic_result<T::Target&, E&>.
  ///
  /// @constrains
  ///   (T t) { *t }
  ///
  /// @note
  ///   Leaves the original basic_result in-place,
  ///   creating a new one with a reference to the original one,
  ///   additionally coercing the success arm of the basic_result via `operator*`.
  constexpr auto indirect_ok() & -> indirect_ok_result {
    if ( static_cast<basic_result<_mutability, T, E>*>(this)->is_ok() ) {
      return indirect_ok_result{in_place_ok, *static_cast<basic_result<_mutability, T, E>*>(this)->unwrap()};
    }
    else {
      return indirect_ok_result{in_place_err, static_cast<basic_result<_mutability, T, E>*>(this)->unwrap_err()};
    }
  }

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
  constexpr auto indirect_err() & -> indirect_err_result {
    if ( static_cast<basic_result<_mutability, T, E>*>(this)->is_ok() ) {
      return indirect_err_result{in_place_ok, static_cast<basic_result<_mutability, T, E>*>(this)->unwrap()};
    }
    else {
      return indirect_err_result{in_place_err, *static_cast<basic_result<_mutability, T, E>*>(this)->unwrap_err()};
    }
  }

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
  constexpr auto indirect() & -> indirect_result {
    if ( static_cast<basic_result<_mutability, T, E>*>(this)->is_ok() ) {
      return indirect_result{in_place_ok, *static_cast<basic_result<_mutability, T, E>*>(this)->unwrap()};
    }
    else {
      return indirect_result{in_place_err, *static_cast<basic_result<_mutability, T, E>*>(this)->unwrap_err()};
    }
  }


  /// @brief
  ///   Converts from basic_result<T, E> &` to basic_result<T::Target const&, E const&>.
  ///
  /// @constrains
  ///   (T t) { *t }
  ///
  /// @note
  ///   Leaves the original basic_result in-place,
  ///   creating a new one with a reference to the original one,
  ///   additionally coercing the success arm of the basic_result via `operator*`.
  constexpr auto indirect_ok() const&  -> const_indirect_ok_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return const_indirect_ok_result{in_place_ok, *static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap()};
    }
    else {
      return const_indirect_ok_result{in_place_err, static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap_err()};
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
  constexpr auto indirect_err() const&  -> const_indirect_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return const_indirect_err_result{in_place_ok, static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap()};
    }
    else {
      return const_indirect_err_result{in_place_err, *static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap_err()};
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
  constexpr auto indirect() const&  -> const_indirect_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return const_indirect_result{in_place_ok, *static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap()};
    }
    else {
      return const_indirect_result{in_place_err, *static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap_err()};
    }
  }


  /// @brief
  ///   Converts from basic_result<T, E> &` to basic_result<dangling<T::Target&>, dangling<E&>>.
  ///
  /// @constrains
  ///   (T t) { *t }
  ///
  /// @note
  ///   Leaves the original basic_result in-place,
  ///   creating a new one with a reference to the original one,
  ///   additionally coercing the success arm of the basic_result via `operator*`.
  ///
  /// @warning
  ///   Contained reference may be exhausted because of original result is rvalue.
  constexpr auto indirect_ok() && -> dangling_indirect_ok_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return dangling_indirect_ok_result{in_place_ok, std::ref(*static_cast<basic_result<_mutability, T, E>*>(this)->unwrap())};
    }
    else {
      return dangling_indirect_ok_result{in_place_err, static_cast<basic_result<_mutability, T, E>*>(this)->unwrap_err()};
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
  constexpr auto indirect_err() && -> dangling_indirect_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return dangling_indirect_err_result{in_place_ok, static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap()};
    }
    else {
      return dangling_indirect_err_result{in_place_err, std::ref(*static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap_err())};
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
  constexpr auto indirect() && -> dangling_indirect_result {
    if ( static_cast<basic_result<_mutability, T, E>*>(this)->is_ok() ) {
      return dangling_indirect_result{in_place_ok, std::ref(*static_cast<basic_result<_mutability, T, E>*>(this)->unwrap())};
    }
    else {
      return dangling_indirect_result{in_place_err, std::ref(*static_cast<basic_result<_mutability, T, E>*>(this)->unwrap_err())};
    }
  }

  constexpr void indirect_ok() const&& = delete;
  constexpr void indirect_err() const&& = delete;
  constexpr void indirect() const&& = delete;

};

/// @impl
///   impl<_mutability, T, E> basic_result<_mutability, T, E>
///   where
///     T: Deref
template <mutability _mutability, class T, class E>
class indirect_friend_injector<basic_result<_mutability, T, E>, 
                            std::enable_if_t<
                              std::conjunction_v<
                                traits::is_dereferencable<T>,
                                std::negation<traits::is_dereferencable<E>>
                            >>>
{
  using indirect_ok_result = basic_result<_mutability, std::remove_reference_t<typename traits::Deref<T>::Target>&, std::remove_reference_t<E>&>;
  using const_indirect_ok_result = basic_result<_mutability, meta::remove_cvr_t<typename traits::Deref<T>::Target> const&, meta::remove_cvr_t<E> const&>;
  using dangling_indirect_ok_result = basic_result<_mutability, dangling<std::reference_wrapper<std::remove_reference_t<typename traits::Deref<T>::Target>>>, dangling<std::reference_wrapper<std::remove_reference_t<E>>>>;
public:
  /// @brief
  ///   Converts from basic_result<T, E> &` to basic_result<T::Target&, E&>.
  ///
  /// @constrains
  ///   (T t) { *t }
  ///
  /// @note
  ///   Leaves the original basic_result in-place,
  ///   creating a new one with a reference to the original one,
  ///   additionally coercing the success arm of the basic_result via `operator*`.
  constexpr auto indirect_ok() & -> indirect_ok_result {
    if ( static_cast<basic_result<_mutability, T, E>*>(this)->is_ok() ) {
      return indirect_ok_result{in_place_ok, *static_cast<basic_result<_mutability, T, E>*>(this)->unwrap()};
    }
    else {
      return indirect_ok_result{in_place_err, static_cast<basic_result<_mutability, T, E>*>(this)->unwrap_err()};
    }
  }
  constexpr void indirect_err() & = delete;
  constexpr void indirect() & = delete;



  /// @brief
  ///   Converts from basic_result<T, E> &` to basic_result<T::Target const&, E const&>.
  ///
  /// @constrains
  ///   (T t) { *t }
  ///
  /// @note
  ///   Leaves the original basic_result in-place,
  ///   creating a new one with a reference to the original one,
  ///   additionally coercing the success arm of the basic_result via `operator*`.
  constexpr auto indirect_ok() const&  -> const_indirect_ok_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return const_indirect_ok_result{in_place_ok, *static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap()};
    }
    else {
      return const_indirect_ok_result{in_place_err, static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap_err()};
    }
  }
  constexpr void indirect_err() const&  = delete;
  constexpr void indirect() const&  = delete;



  /// @brief
  ///   Converts from basic_result<T, E> &` to basic_result<dangling<T::Target&>, dangling<E&>>.
  ///
  /// @constrains
  ///   (T t) { *t }
  ///
  /// @note
  ///   Leaves the original basic_result in-place,
  ///   creating a new one with a reference to the original one,
  ///   additionally coercing the success arm of the basic_result via `operator*`.
  ///
  /// @warning
  ///   Contained reference may be exhausted because of original result is rvalue.
  constexpr auto indirect_ok() && -> dangling_indirect_ok_result {
    if ( static_cast<basic_result<_mutability, T, E>*>(this)->is_ok() ) {
      return dangling_indirect_ok_result{in_place_ok, std::ref(*static_cast<basic_result<_mutability, T, E>*>(this)->unwrap())};
    }
    else {
      return dangling_indirect_ok_result{in_place_err, static_cast<basic_result<_mutability, T, E>*>(this)->unwrap_err()};
    }
  }
  constexpr void indirect_err() && = delete;
  constexpr void indirect() && = delete;

  constexpr void indirect_ok() const&& = delete;
  constexpr void indirect_err() const&& = delete;
  constexpr void indirect() const&& = delete;

};

/// @impl
///   impl<_mutability, T, E> basic_result<_mutability, T, E>
///   where
///     E: Deref
template <mutability _mutability, class T, class E>
class indirect_friend_injector<basic_result<_mutability, T, E>, 
                            std::enable_if_t<
                              std::conjunction_v<
                                std::negation<traits::is_dereferencable<T>>,
                                traits::is_dereferencable<E>
                            >>>
{
  using indirect_err_result = basic_result<_mutability, std::remove_reference_t<T>&, std::remove_reference_t<typename traits::Deref<E>::Target>&>;
  using const_indirect_err_result = basic_result<_mutability, meta::remove_cvr_t<T> const&, meta::remove_cvr_t<typename traits::Deref<E>::Target> const&>;
  using dangling_indirect_err_result = basic_result<_mutability, dangling<std::remove_reference_t<T>&>, dangling<std::remove_reference_t<typename traits::Deref<E>::Target>&>>;
public:
  constexpr void indirect_ok() & = delete;
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
  constexpr auto indirect_err() & -> indirect_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return indirect_err_result{in_place_ok, static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap()};
    }
    else {
      return indirect_err_result{in_place_err, *static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap_err()};
    }
  }
  constexpr void indirect() & = delete;



  constexpr void indirect_ok() const&  = delete;
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
  constexpr auto indirect_err() const&  -> const_indirect_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return const_indirect_err_result{in_place_ok, static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap()};
    }
    else {
      return const_indirect_err_result{in_place_err, *static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap_err()};
    }
  }
  constexpr void indirect() const&  = delete;



  constexpr void indirect_ok() && = delete;
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
  constexpr auto indirect_err() && -> dangling_indirect_err_result {
    if ( static_cast<basic_result<_mutability, T, E>*>(this)->is_ok() ) {
      return dangling_indirect_err_result{in_place_ok, static_cast<basic_result<_mutability, T, E>*>(this)->unwrap()};
    }
    else {
      return dangling_indirect_err_result{in_place_err, std::ref(*static_cast<basic_result<_mutability, T, E>*>(this)->unwrap_err())};
    }
  }
  constexpr void indirect() && = delete;

  constexpr void indirect_ok() const&& = delete;
  constexpr void indirect_err() const&& = delete;
  constexpr void indirect() const&& = delete;

};


} // !namespace mitama
#endif
