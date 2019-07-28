**ok_or(E err) -> result<T, E>**

```cpp
template <class E>
auto maybe<T>::ok_or(E&& err) const& -> result<T, E> ;

template <class E>
auto maybe<T>::ok_or(E&& err) && -> result<std::remove_reference_t<T>, E> ;

auto maybe<T>::ok_or() const& -> result<T, std::monostate> ;

auto maybe<T>::ok_or() && -> result<std::remove_reference_t<T>, std::monostate> ;
```

Transforms the `maybe<T>` into a `result<T, E>`, mapping `just(v)` to `success(v)` and `nothing` to `failure(err)`.

Arguments passed to ok_or are eagerly evaluated; if you are passing the result of a function call, it is recommended to use ok_or_else, which is lazily evaluated.

**Example**

```cpp
maybe x = just("foo"s);
assert(x.ok_or(0) == success("foo"s));

maybe<std::string> y = nothing;
assert(y.ok_or(0) == failure(0));

assert(y.ok_or() == failure<>());
```

