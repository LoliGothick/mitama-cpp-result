**is_just()**

```cpp
constexpr bool maybe<T>::is_just() const noexcept ;
```

Returns `true` if the maybe has same value.

**Example**

```cpp
maybe<int> x = just(2);
assert( x.is_just() );

maybe<int> y = nothing;
assert( ! y.is_just() );
```
