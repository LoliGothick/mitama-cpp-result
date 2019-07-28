**result&lt;T, E&gt;::unwrap_or_else() -> T**

```cpp
template <class O>
auto basic_result<_, T, E>::unwrap_or_else(O && op) const noexcept
  -> std::enable_if_t<std::is_invocable_r_v<T, O, E>, T> ;
```

Unwraps a result, yielding the content of an `success`. If the value is an `failure` then it calls `op` with its value.

**Example**

```cpp
auto count = [](std::stringx) -> size_t { return x.size(); };
assert_eq(success(2).unwrap_or_else(count), 2);
assert_eq(failure("foo"s).unwrap_or_else(count), 3);
```

