**result&lt;T, E&gt;::err() -> maybe&lt;E&gt;**

```cpp
constexpr auto result<T, E>::err() &
    -> maybe<const E> ;

constexpr auto result<T, E>::err() const &
    -> maybe<const E> ;

constexpr auto result<T, E>::err() &&
    -> maybe<const E> ;

constexpr auto mut_result<T, E>::err() &
    -> maybe<E> ;

constexpr auto mut_result<T, E>::err() const &
    -> maybe<const E> ;

constexpr auto mut_result<T, E>::err() &&
    -> maybe<E> ;
```

Converts from `basic_result` to `maybe`.

Converts self into an `maybe`, and discarding the success value, if any.

Note that these functions propagate mutability to optional element types.

**Example**

```cpp
result<unsigned, std::string> x = success(2);
assert(x.err() == nothing);
result<unsigned, std::string> y = failure("Nothing here");
assert(y.err() == just("Nothing here"s));
```

**Remarks**

If self is rvalue and `E` is a reference type,
this function returns `maybe<dangling<std::reference_wrapper<std::remove_reference_t<E>>>>`.
