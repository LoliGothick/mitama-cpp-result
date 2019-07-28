**is_nothing()**

```cpp
constexpr bool maybe<T>::is_nothing() const noexcept ;
```

Returns `true` if the maybe has not same value.

**Example**

```cpp
maybe<int> x = just(2);
assert( ! x.is_nothing() );

maybe<int> y = nothing;
assert( y.is_nothing() );
```
