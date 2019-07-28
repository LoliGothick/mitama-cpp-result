# Constructors

## default constructor (deleted) [0/16]

Default constructor is not permitted.

## explicit copy constructor for convertible basic_result [1/16]

```cpp
template<mutability _, class U, class F>
explicit constexpr(see below)
basic_result::basic_result(basic_result<_, U, F> const& res)
noexcept(see below)
```

**Effects**

Initialize a retention variant from what `res` holds by copy.

**Exceptions**

Any exception thrown by the selected constructor of `T` or `E`.

If `is_nothrow_copy_constructible_v<T, U> && is_nothrow_copy_constructible_v<E, F>` is true, this constructor shall be a nothrow constructor.

**Remarks**

This constructor shall not participate in overload resolution unless

- `std::is_constructible<T, U>` is true,
- `std::is_constructible<E, F>` is true and
- `std::disjunction<std::negation<std::is_convertible<F, E>>, std::negation<std::is_convertible<U, T>>>` is true.

If `is_trivially_copy_constructible_v<T, U> && is_trivially_copy_constructible_v<E, F>` is true, this constructor shall be a constexpr constructor.

## non-explicit copy constructor for convertible basic_result [2/16]

```cpp
template<mutability _, class U, class F>
constexpr(see below)
basic_result::basic_result(basic_result<_, U, F> const& res)
noexcept(see below)
```
**Effects**

Initialize a retention variant from what `res` holds by copy.

**Exceptions**

Any exception thrown by the selected constructor of `T` or `E`.

If `is_nothrow_copy_constructible_v<T, U> && is_nothrow_copy_constructible_v<E, F>` is true, this constructor shall be a nothrow constructor.

**Remarks**

This constructor shall not participate in overload resolution unless

- `std::is_constructible<T, U>` is true,
- `std::is_constructible<E, F>` is true,
- `std::is_convertible<F, E>` is true and,
- `std::is_convertible<U, T>>` is true.

If `is_trivially_copy_constructible_v<T, U> && is_trivially_copy_constructible_v<E, F>` is true, this constructor shall be a constexpr constructor.

## explicit move constructor for convertible basic_result [3/16]

```cpp
template<mutability _, class U, class F>
explicit constexpr(see below)
basic_result::basic_result(basic_result<_, U, F>&& res)
noexcept(see below)
```

**Effects**

Initialize a retention variant from what `res` holds by move.

**Exceptions**

Any exception thrown by the selected constructor of `T` or `E`.

If `is_nothrow_move_constructible_v<T, U> && is_nothrow_move_constructible_v<E, F>` is true, this constructor shall be a nothrow constructor.

**Remarks**

This constructor shall not participate in overload resolution unless

- `std::is_constructible<T, U>` is true,
- `std::is_constructible<E, F>` is true and,
- `std::disjunction<std::negation<std::is_convertible<F, E>>, std::negation<std::is_convertible<U, T>>>` is true.

If `is_trivially_move_constructible_v<T, U> && is_trivially_move_constructible_v<E, F>` is true, this constructor shall be a constexpr constructor.

## non-explicit copy constructor for convertible basic_result [4/16]

```cpp
template<mutability _, class U, class F>
constexpr(see below)
basic_result::basic_result(basic_result<_, U, F>&& res)
noexcept(see below)
```
**Effects**

Initialize a retention variant from what `res` holds by move.

**Exceptions**

Any exception thrown by the selected constructor of `T` or `E`.

If `is_nothrow_move_constructible_v<T, U> && is_nothrow_move_constructible_v<E, F>` is true, this constructor shall be a nothrow constructor.

**Remarks**

This constructor shall not participate in overload resolution unless

- `std::is_constructible<T, U>` is true,
- `std::is_constructible<E, F>` is true,
- `std::is_convertible<F, E>` is true and,
- `std::is_convertible<U, T>>` is true.

If `is_trivially_move_constructible_v<T, U> && is_trivially_move_constructible_v<E, F>` is true, this constructor shall be a constexpr constructor.

## non-explicit copy constructor from success [5/16]

```cpp
template<class U >
basic_result::basic_result(success<U> const& ok)
noexcept(see below)
```

**Effects**

Initializes the contained variant as if in-place-initializing an object of type `success<T>`.

**Exceptions**

Any exception thrown by the selected constructor of `T`.

If `is_nothrow_move_constructible_v<T, U>` is true, this constructor shall be a nothrow constructor.

**Remarks**

This constructor shall not participate in overload resolution unless `is_constructible_v<T, U> && is_convertible_v<U, T>` is true.

## explicit copy constructor from success [6/16]

```cpp
template<class U >
explicit basic_result::basic_result(success<U> const& ok)
noexcept(see below)
```

**Effects**

Initializes the contained variant as if in-place-initializing an object of type `success<T>`.

**Exceptions**

Any exception thrown by the selected constructor of `T`.

If `is_nothrow_move_constructible_v<T, U>` is true, this constructor shall be a nothrow constructor.

**Remarks**

This constructor shall not participate in overload resolution unless `is_constructible_v<T, U> && !is_convertible_v<U, T>` is true.

## non-explicit move constructor from success [7/16]

```cpp
template<class U >
basic_result::basic_result(success<U>&& ok)
noexcept(see below)
```

**Effects**

Initializes the contained variant as if in-place-initializing an object of type `success<T>`.

**Exceptions**

Any exception thrown by the selected constructor of `T`.

If `is_nothrow_move_constructible_v<T, U>` is true, this constructor shall be a nothrow constructor.

**Remarks**

This constructor shall not participate in overload resolution unless `is_constructible_v<T, U> && is_convertible_v<U, T>` is true.

## explicit move constructor from success [8/16]

```cpp
template<class U >
explicit basic_result::basic_result(success<U>&& ok)
noexcept(see below)
```

**Effects**

Initializes the contained variant as if in-place-initializing an object of type `success<T>`.

**Exceptions**

Any exception thrown by the selected constructor of `T`.

If `is_nothrow_move_constructible_v<T, U>` is true, this constructor shall be a nothrow constructor.

**Remarks**

This constructor shall not participate in overload resolution unless `is_constructible_v<T, U> && !is_convertible_v<U, T>` is true.

## non-explicit copy constructor from failure [9/16]

```cpp
template <class F>
basic_result::basic_result(failure<F> const& err)
noexcept(see below)
```

**Effects**

Initializes the contained variant as if in-place-initializing an object of type `failure<E>`.

**Exceptions**

Any exception thrown by the selected constructor of `E`.

If `is_nothrow_move_constructible_v<E, F>` is true, this constructor shall be a nothrow constructor.

**Remarks**

This constructor shall not participate in overload resolution unless `is_constructible_v<E, F> && is_convertible_v<F, E>` is true.

## explicit copy constructor from failure [10/16]

```cpp
template <class F>
explicit basic_result::basic_result(failure<F> const& err)
noexcept(see below)
```

**Effects**

Initializes the contained variant as if in-place-initializing an object of type `failure<E>`.

**Exceptions**

Any exception thrown by the selected constructor of `E`.

If `is_nothrow_move_constructible_v<E, F>` is true, this constructor shall be a nothrow constructor.

**Remarks**

This constructor shall not participate in overload resolution unless `is_constructible_v<E, F> && !is_convertible_v<F, E>` is true.

## non-explicit move constructor from failure [11/16]

```cpp
template <class F>
basic_result::basic_result(failure<F>&& err)
noexcept(see below)
```

**Effects**

Initializes the contained variant as if in-place-initializing an object of type `failure<E>`.

**Exceptions**

Any exception thrown by the selected constructor of `E`.

If `is_nothrow_move_constructible_v<E, F>` is true, this constructor shall be a nothrow constructor.

**Remarks**

This constructor shall not participate in overload resolution unless `is_constructible_v<E, F> && is_convertible_v<F, E>` is true.

## explicit move constructor from failure [12/16]

```cpp
template <class F>
explicit basic_result::basic_result(failure<F>&& err)
noexcept(see below)
```

**Effects**

Initializes the contained variant as if in-place-initializing an object of type `failure<E>&&`.

**Exceptions**

Any exception thrown by the selected constructor of `E`.

If `is_nothrow_move_constructible_v<E, F>` is true, this constructor shall be a nothrow constructor.

**Remarks**

This constructor shall not participate in overload resolution unless `is_constructible_v<E, U> && !is_convertible_v<U, E>` is true.

## emplace constructor for successful results [13/16]

```cpp
template <class... Args>
explicit basic_result::basic_result(in_place_ok_t, Args&&... args)
noexcept(see below)
```

**Effects**

Initializes the contained variant as if in-place-initializing an object of type with expression `(success<T>(std::forward<Args>(args)...))`.

**Exceptions**

Any exception thrown by the selected constructor of `T`.

If `is_nothrow_move_constructible_v<T, Args&&...>` is true, this constructor shall be a nothrow constructor.

**Example**

```cpp
using my_result = result<std::tuple<int, int>, std::string>;
auto res = my_result( mitama::in_place_ok, 1, 1 );
// same as `my_result(success(std::tuple{1,1}))`
```

## emplace constructor for unsuccessful results [14/16]

```cpp
template <class... Args>
explicit basic_result::basic_result(in_place_err_t, Args&&... args)
```

**Effects**

Initializes the contained variant as if in-place-initializing an object of type with expression `(failure<E>(std::forward<Args>(args)...))`.

**Exceptions**

Any exception thrown by the selected constructor of `E`.

If `is_nothrow_move_constructible_v<E, Args&&...>` is true, this constructor shall be a nothrow constructor.

**Example**

```cpp
using my_result = result<int, std::string>;
auto res = my_result( mitama::in_place_err, 'a', 5 ); // failure("aaaaa")
```

## emplace constructor with initializer_list for successful results [15/16]

```cpp
template <class U , class... Args>
explicit basic_result::basic_result(in_place_ok_t, 
                                    std::initializer_list<U> il,
                                    Args &&... args)
```

**Effects**

Initializes the contained variant as if in-place-initializing an object of type with expression `(success<T>(il, std::forward<Args>(args)...))`.

**Exceptions**

Any exception thrown by the selected constructor of T.

**Example**

```cpp
using my_result = result<std::vector<int>, std::string>;
auto res = my_result( in_place_ok, {1,2,3,4}, std::allocator<int>()); // success([1,2,3,4])
```

## emplace constructor with initializer_list for unsuccessful results [16/16]

```cpp
template <class U , class... Args>
explicit basic_result::basic_result(in_place_ok_t ,
                                    std::initializer_list<U> il,
                                    Args&&... args)
```

**Effects**

Initializes the contained variant as if in-place-initializing an object of type with expression `(failure<E>(il, std::forward<Args>(args)...))`.

**Exceptions**

Any exception thrown by the selected constructor of `E`.

**Example**

```cpp
using my_result = result<std::string, std::vector<int>>;
auto res = my_result( in_place_err, {1,2,3,4}); // failure([1,2,3,4])
```

# Assignment operators


## copy assignment operator [1/6]

```cpp
template <mutability _, class U, class F,
          where<std::is_constructible<T, U>,
               std::is_constructible<E, F>
        > = required>
constexpr basic_result& basic_result::operator=(basic_result<_, U, F> const& res)
```

**Effects**

Destroy the contained value and replace it with the one contained in `res`.

**Exceptions**

Any exception thrown by the selected constructor of `T` or `E`.

**Remark**

This operator fails by static assertion unless self is mutable.

## move assignment operator [2/6]

```cpp
template <mutability _, class U, class F,
         where<std::is_constructible<T, U>,
               std::is_constructible<E, F>
        > = required>
constexpr basic_result& basic_result::operator=(basic_result<_, U, F>&& res)
```

**Effects**

Destroy the contained value and replace it with the one contained in `res`.

**Exceptions**

Any exception thrown by the selected constructor of `T` or `E`.

**Remark**

This operator fails by static assertion unless self is mutable.

## copy assignment operator for success [3/6]

```cpp
  template <class U,
            where<std::is_constructible<T, U>> = required>
  constexpr basic_result& basic_result::operator=(success<U> const& _ok)
```

**Effects**

Destroy the contained value and replace it with the successful value `_ok`.

**Exceptions**

Any exception thrown by the selected constructor of `T`.

**Remark**

This operator fails by static assertion unless self is mutable.

## move assignment operator for success [4/6]

```cpp
  template <class U,
            where<std::is_constructible<T, U>> = required>
  constexpr basic_result& basic_result::operator=(success<U>&& _ok)
```

**Effects**

Destroy the contained value and replace it with the successful value `_ok`.

**Exceptions**

Any exception thrown by the selected constructor of `T`.

**Remark**

This operator fails by static assertion unless self is mutable.

## copy assignment operator for failure [5/6]

```cpp
  template <class F,
            where<std::is_constructible<E, F>> = required>
  constexpr basic_result& basic_result::operator=(failure<F> const& _err)
```

**Effects**

Destroy the contained value and replace it with the unsuccessful value `_err`.

**Exceptions**

Any exception thrown by the selected constructor of `E`.

**Remark**

This operator fails by static assertion unless self is mutable.

## move assignment operator for failure [6/6]

```cpp
  template <class F,
            where<std::is_constructible<E, F>> = required>
  constexpr basic_result& basic_result::operator=(failure<F>&& _err)
```

**Effects**

Destroy the contained value and replace it with the unsuccessful value `_err`.

**Exceptions**

Any exception thrown by the selected constructor of `E`.

**Remark**

This operator fails by static assertion unless self is mutable.
