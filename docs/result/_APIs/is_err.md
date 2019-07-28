**result&lt;T, E&gt;::is_err() -> bool**

```cpp
constexpr bool basic_result<_, T, E>::is_err() const noexcept ;
```

Returns true if the result is failure.

**Example**

```cpp
result<uint32_t, std::string> x = success(-3);
assert_eq(x.is_err(), false);
result<uint32_t, std::string> y = failure("Some error message");
assert_eq(y.is_err(), true);
```
