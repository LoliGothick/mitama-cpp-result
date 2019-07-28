**operator== [1/3]**

```cpp
template <mutability _, class U , class F>
bool basic_result<_, T, E>::operator==(basic_result<_, U, F> const& rhs) const& ;
```

Equality comparison for `basic_result<_, T, E>` and `basic_result<_, U, F>`.

**Remarks**

This operator shall not participate in overload resolution unless `std::declval<T const&>() == std::declval<U const&>()` is valid expression and `std::declval<E const&>() == std::declval<F const&>()` is valid expression.

**operator==() [2/3]**

```cpp
template <class U>
bool result::operator==(success<U> const& rhs) const&
```

Equality comparison for `basic_result<_, T, E>` and `success<U>`.

**Returns**

`true` if this has `success` value `this->unwrap()` equals `rhs` value, otherwise `false`.

**Remarks**

This operator shall not participate in overload resolution unless `std::declval<T const&>() == std::declval<U const&>()` is valid expression.


**operator==() [3/3]**

```cpp
template <class F >
bool basic_result::operator==(failure<F> const& rhs) const&
```

Equality comparison for `basic_result<_, T, E>` and `failure<F>`.

**Returns**

`true` if this has `failure` value `this->unwrap_err()` equals `rhs` value, otherwise `false`.

**Remarks**

This operator shall not participate in overload resolution unless `std::declval<E const&>() == std::declval<F const&>()` is valid expression.

