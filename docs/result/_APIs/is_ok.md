**result&lt;T, E&gt;::is_ok() -> bool**

```cpp
constexpr bool basic_result<_, T, E>::is_ok() const noexcept ;
```

Returns true if the result is `success`.

**Example**

```cpp
result<uint32_t, std::string> x = success(-3);
assert(x.is_ok(), true);
result<uint32_t, std::string> y = failure("Some error message");
assert(y.is_ok(), false);
```
