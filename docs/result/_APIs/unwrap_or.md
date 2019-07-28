**result&lt;, E&gt;::unwrap_or(T const& optb) -> T**

```cpp
auto basic_result<_, T, E>::unwrap_or(T const& optb) const noexcept
```

Unwraps a result, yielding the content of an `success`. Else, it returns `optb`.

Arguments passed to unwrap_or are eagerly evaluated; if you are passing the result of a function call, it is recommended to use `unwrap_or_else`, which is lazily evaluated.

**Example**

```cpp
result<unsigned, unsigned> err = failure(2);
result<unsigned, unsigned> ok = success(2);
assert_eq(ok.unwrap_or(1u), 2u);
assert_eq(err.unwrap_or(1u), 1u);
```
