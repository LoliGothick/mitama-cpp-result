**replace(T& value) requires T is copyable -> maybe&lt;T&gt;**

```cpp
maybe<T> replace(T& value) & ;
```

Replaces the actual value in the maybe by the value given in parameter, returning the old value if present, leaving a `just` value in its place without deinitializing either one.

**Example**

```cpp
{
    maybe x = just(2);
    auto old = x.replace(5);
    REQUIRE(x == just(5));
    REQUIRE(old == just(2));
}
{
    maybe<int> x = nothing;
    auto old = x.replace(3);
    REQUIRE(x == just(3));
    REQUIRE(old == nothing);
}
```
