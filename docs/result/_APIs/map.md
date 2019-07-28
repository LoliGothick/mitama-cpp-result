**result&lt;T, E&gt;::map(O op) -> result&lt;U, E&gt;**
**where O: T -> U**

```cpp
constexpr auto basic_result<_, T, E>::map(O && op)const &
  -> std::enable_if_t<std::is_invocable_v<O, T>, result<std::invoke_result_t<O, T>, E>> ;
```

Maps a `result<T, E>` to `result<U, E>` by applying a function to a contained `success` value, leaving an `failure` value untouched.

This function can be used to compose the results of two functions.

**Example**

```cpp
std::string line = "1,3,5,7";
for (auto num : split(line, ","))
{
  if (auto res = parse<int>(num).map(_1 * 2); res.is_ok())
  {
    assert_true(res.ok().value() % 2 == 0);
  }
}
```
