**expect(msg)**

```cpp
auto maybe<T>::expect(std::string_view msg) & -> T& ;

auto maybe<T>::expect(std::string_view msg) const& -> T const& ;

auto maybe<T>::expect(std::string_view msg) && -> T&& ;
```

Unwraps a maybe, yielding the content of an `just`.

**Exception**

Raise `mitama::runtime_panic` if the value is a `nothing` with a custom panic message provided by `msg`.

**Example**

```cpp
{
    maybe x = just("value"s);
    assert( x.expect("the world is ending"s) == "value"s );
}
try {
    maybe<int> x = nothing;
    x.expect("the world is ending"); // panics with `the world is ending`
}
catch ( mitama::runtime_panic cosnt & panic ) {
    std::err << panic.what() << std::endl; // runtime panicked at 'the world is ending'
}
```
