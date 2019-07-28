**result&lt;T, E&gt;::and_then(O op) -> result&lt;U, E&gt;**
**where O: T -> result&lt;U, E&gt;**

```cpp
template <class O>
constexpr auto basic_result<_, T, E>::and_then(O&& op)
  -> std::invoke_result<O&&, T> ;
```

Calls `op` if the result is success, otherwise returns the failure value of self.

This function can be used for control flow based on result values.

**Remarks**

This constructor shall not participate in overload resolution unless `is_result_with_v<std::invoke_result_t<O, T>, failure<E>>` is true.

**Example**

```cpp
auto sq = [](unsigned x) -> result<unsigned, unsigned> { return success(x * x); };
auto err = [](unsigned x) -> result<unsigned, unsigned> { return failure(x); };
assert(success(2u).and_then(sq).and_then(sq) == success(16u));
assert(success(2u).and_then(sq).and_then(err) == failure(4u));
assert(success(2u).and_then(err).and_then(sq) == failure(2u));
assert(failure(3u).and_then(sq).and_then(sq) == failure(3u));
```
