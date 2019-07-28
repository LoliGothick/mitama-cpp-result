**result&lt;T, E&gt;::unwrap_err() -> E&**

```cpp
auto result<T, E>::unwrap_err() &
  -> const E ;

auto result<T, E>::unwrap_err() const&
  -> const E ;

auto result<T, E>::unwrap_err() &&
  -> const E ;

auto mut_result<T, E>::unwrap_err() &
  -> E ;

auto mut_result<T, E>::unwrap_err() const&
  -> const E ;

auto mut_result<T, E>::unwrap_err() &&
  -> E ;
```

Unwraps a result, yielding the content of an `failure`.

**Exception**

Raise `mitama::runtime_panic` if a result is containing `success` value.

**Remarks**

If self is rvalue and `E` is a reference type,
this function returns `boost::optional<dangling<std::reference_wrapper<std::remove_reference_t<E>>>>`.


**Example**

```cpp
try {
  result<unsigned, std::string> x = success(2);
  x.unwrap_err(); // panics with `2`
}
catch (runtime_panic const &panic)
{
   std::err << panic.what() << std::endl; // 2
}
{
  result<unsigned, std::string> x = failure("emergency failure"s);
  assert_eq(x.unwrap_err(), "emergency failure"s);
}
```
