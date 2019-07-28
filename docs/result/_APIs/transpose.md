**result&lt;&lt;T&gt;, E&gt;::transpose() -> maybe&lt;result&lt;T, E&gt;&lt;**

```cpp
auto basic_result<_, std::optional<T>, E>::transpose()
  -> std::optional<basic_result<_, T, E>> ;

auto basic_result<_, boost::optional<T>, E>::transpose()
  -> boost::optional<basic_result<_, T, E>> ;
```

Transposes a `result` of an `optional` into an `optional` of a `result`.


`success(None)` will be mapped to `None`.
`success(Some(v))` and `failure(v)` will be mapped to `optional(success(v))` and `optional(failure(v))`.

**Example**

```cpp
struct SomeError{};

result<std::optional<int>, SomeError> x = success(std::optional(5));
std::optional<result<int, SomeError>> y = std::optional(result<int, SomeError>(success(5)));
assert_eq!(x.transpose(), y);
```
