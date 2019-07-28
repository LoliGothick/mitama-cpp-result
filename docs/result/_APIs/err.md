**result&lt;T, E&gt;::err() -> maybe&lt;E&gt;**

```cpp
constexpr auto result<T, E>::err() &
    -> boost::optional<const E> ;

constexpr auto result<T, E>::err() const &
    -> boost::optional<const E> ;

constexpr auto result<T, E>::err() &&
    -> boost::optional<const E> ;

constexpr auto mut_result<T, E>::err() &
    -> boost::optional<E> ;

constexpr auto mut_result<T, E>::err() const &
    -> boost::optional<const E> ;

constexpr auto mut_result<T, E>::err() &&
    -> boost::optional<E> ;
```

Converts from `basic_result` to `boost::optional`.

Converts self into an `boost::optional`, and discarding the success value, if any.

Note that these functions propagate mutability to optional element types.

**Example**

```cpp
result<unsigned, std::string> x = success(2);
assert(x.err() == None);
result<unsigned, std::string> y = failure("Nothing here");
assert(y.err() == Some("Nothing here"));
```

**Remarks**

If self is rvalue and `E` is a reference type,
this function returns `boost::optional<dangling<std::reference_wrapper<std::remove_reference_t<E>>>>`.
