**maybe&lt;T&&gt;::cloned() requires T is copyable -> maybe&lt;T&gt;**

```cpp
auto maybe<T&>::cloned() & -> maybe<T>;
```

Maps an `maybe<T&>` to an `maybe<T>` by deep copying the contents of the maybe.

**Example**

```cpp
auto x = 12;
maybe opt_x = just(x);
assert(opt_x == just(12));
assert(&(opt_x.unwrap()) == &x);
auto cloned = opt_x.cloned();
assert(cloned == just(12));
assert(&(cloned.unwrap()) != &x);
```