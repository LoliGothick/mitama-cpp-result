**result&lt;T, E&gt;::unwrap() -> E&**

```cpp
auto result<T, E>::unwrap() &
  -> const T ;

auto result<T, E>::unwrap() const&
  -> const T ;

auto result<T, E>::unwrap() &&
  -> const T ;

auto mut_result<T, E>::unwrap() &
  -> T ;

auto mut_result<T, E>::unwrap() const&
  -> const T ;

auto mut_result<T, E>::unwrap() &&
  -> T ;
```

Unwraps a result, yielding the content of an `success`.

**Exception**

Raise `mitama::runtime_panic` if a result is containing `failure` value.

**Example**

```cpp
{
  result<unsigned, std::string> x = success(2);
  assert_eq(x.unwrap(), 2);
}
try {
  result<unsigned, std::string> x = failure("emergency failure"s);
  x.unwrap(); // panics with `emergency failure`
}
catch ( mitama::runtime_panic cosnt & panic ) {
  std::err << panic.what() << std::endl; // `emergency failure`
}
```
