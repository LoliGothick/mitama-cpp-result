**maybe&lt;maybe&lt;T&gt;&gt;::flatten() -> maybe&lt;T&gt;**

```cpp
auto maybe<maybe<T>>::transpose() -> maybe<T> ;
```

Converts from `maybe<maybe<T>>` to `maybe<T>`.

**Example**

```cpp
maybe<maybe<int>> x = just(just(6));
assert(just(6) == x.flatten());

maybe<maybe<int>> y = just(nothing);
assert(nothing == y.flatten());

maybe<maybe<int>> z = nothing;
assert(nothing == z.flatten());

// Flattening once only removes one level of nesting:
maybe<maybe<maybe<int>>> nest = just(just(just(6)));
assert(just(6) == nest.flatten().flatten());
```