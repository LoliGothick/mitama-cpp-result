**unwrap()**

```cpp
auto maybe<T>::unwrap() & -> T& ;

auto maybe<T>::unwrap() const& -> T const& ;

auto maybe<T>::unwrap() && -> T&& ;
```

Unwraps a maybe, yielding the content of an `just`.

**Exception**

Raise `mitama::runtime_panic` if a maybe has not `just` value.

**Example**

```cpp
{
  maybe x = just("air"s);
  assert(x.unwrap() == "air"s);
}
try {
  maybe<int> x = nothing;
  x.unwrap(); // raise an exception
}
catch ( mitama::runtime_panic cosnt & panic ) {
  std::err << panic.what() << std::endl; // runtime panicked at 'called `maybe::unwrap()` on a `nothing` value'
}
```
