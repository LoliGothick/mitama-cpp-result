**result&lt;T, E&gt;::ok() -> maybe&lt;T&gt;**

```cpp
constexpr auto result<T, E>::ok() &
    -> maybe<const T> ;

constexpr auto result<T, E>::ok() const &
    -> maybe<const T> ;

constexpr auto result<T, E>::ok() &&
    -> maybe<const T> ;

constexpr auto mut_result<T, E>::ok() &
    -> maybe<T> ;

constexpr auto mut_result<T, E>::ok() const &
    -> maybe<const T> ;

constexpr auto mut_result<T, E>::ok() &&
    -> maybe<T> ;
```

Converts from `basic_result` to `maybe`.

Converts self into an `maybe`, and discarding the failure value, if any.

Note that these functions propagate mutability to optional element types.

**Example**

```cpp
result<unsigned, std::string> x = success(2);
assert(x.ok() == just(2));
result<unsigned, std::string> y = failure("Nothing here");
assert(y.ok() == nothing);
```

**Remarks**

If self is rvalue and `T` is a reference type,
this function returns `maybe<dangling<std::reference_wrapper<std::remove_reference_t<T>>>>`.