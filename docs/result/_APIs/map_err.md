**result&lt;T, E&gt;::map_err(O op) -> result&lt;T, F&gt;**
**where O: E -> F**

```cpp
template <class O>
constexpr auto basic_result<_, T, E>::map_err(O && op) const &
  -> std::enable_if_t<std::is_invocable_v<O, E>, result<T, std::invoke_result_t<O, E>>> ;
```

Maps a `result<T, E>` to `result<T, F>` by applying a function to a contained failure value, leaving an success value untouched.

This function can be used to pass through a successful result while handling an error.

**Example**

```cpp
auto stringify = [](unsigned x) -> std::string{
  return "error code: "s + std::to_string(x);
};
result<unsigned, unsigned> x = success(2);
assert_eq(x.map_err(stringify), success(2u));
result<unsigned, unsigned> y = failure(13);
assert_eq(y.map_err(stringify), failure("error code: 13"s));
```
