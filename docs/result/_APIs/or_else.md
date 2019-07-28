**result&lt;T, E&gt;::or_else(O op) ->result&lt;T, F&gt;**
**where O: E -> result&lt;T, F&gt;**

```cpp
template <class O>
constexpr auto basic_result<_, T, E>::or_else(O && op) const &
  -> std::invoke_result<O&&, E> ;
```

Calls `op` if the result is `failure`, otherwise returns the `success` value of self.

This function can be used for control flow based on result values.

**Remarks**

This constructor shall not participate in overload resolution unless `is_result_with_v<std::invoke_result_t<O, T>, success<T>>` is true.

**Example**

```cpp
auto sq = [](unsigned x) -> result<unsigned, unsigned> { return success(x * x); };
auto err = [](unsigned x) -> result<unsigned, unsigned> { return failure(x); };
assert_eq(success(2).or_else(sq).or_else(sq), success(2u));
assert_eq(success(2).or_else(err).or_else(sq), success(2u));
assert_eq(failure(3).or_else(sq).or_else(err), success(9u));
assert_eq(failure(3).or_else(err).or_else(err), failure(3u));
```
