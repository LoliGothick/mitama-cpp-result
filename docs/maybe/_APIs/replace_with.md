**replace_with(F f, Args... args) -> maybe&lt;T&gt;**

```cpp
template <class F, class... Args>
maybe<T> replace_with(F&& f, Args&&... args) & ;
```

Replaces the actual value in the maybe by expression `std::invoke(std::forward<F>(f), std::forward<Args>(args)...)`, returning the old value if present, leaving a `just` value in its place without deinitializing either one.

**Example**

```cpp
{
    maybe x = just(2);
    auto old = x.replace_with([]{ return 5; });
    REQUIRE(x == just(5));
    REQUIRE(old == just(2));
}
{
    maybe<int> x = nothing;
    auto old = x.replace_with(&std::string::size, "foo"s);
    REQUIRE(x == just(3));
    REQUIRE(old == nothing);
}
```
