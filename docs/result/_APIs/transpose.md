**result&lt;&lt;T&gt;, E&gt;::transpose() -> maybe&lt;result&lt;T, E&gt;&gt;**

```cpp
auto basic_result<_, maybe<T>, E>::transpose()
  -> maybe<basic_result<_, T, E>> ;

auto basic_result<_, maybe<T>, E>::transpose()
  -> maybe<basic_result<_, T, E>> ;
```

Transposes a `result` of an `maybe` into an `maybe` of a `result`.


`success(nothing)` will be mapped to `nothing`.
`success(just(v))` and `failure(v)` will be mapped to `just(success(v))` and `just(failure(v))`.

**Example**

```cpp
struct SomeError{};

result<maybe<int>, SomeError> x = success(just(5));
maybe<result<int, SomeError>> y = just(success(5));
assert_eq!(x.transpose(), y);
```
