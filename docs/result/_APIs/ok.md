**result&lt;T, E&gt;::ok() -> maybe&lt;T&gt;**

```cpp
constexpr auto result<T, E>::ok() &
    -> boost::optional<const T> ;

constexpr auto result<T, E>::ok() const &
    -> boost::optional<const T> ;

constexpr auto result<T, E>::ok() &&
    -> boost::optional<const T> ;

constexpr auto mut_result<T, E>::ok() &
    -> boost::optional<T> ;

constexpr auto mut_result<T, E>::ok() const &
    -> boost::optional<const T> ;

constexpr auto mut_result<T, E>::ok() &&
    -> boost::optional<T> ;
```

Converts from `basic_result` to `boost::optional`.

Converts self into an `boost::optional`, and discarding the failure value, if any.

Note that these functions propagate mutability to optional element types.

**Example**

```cpp
result<unsigned, std::string> x = success(2);
assert(x.ok() == Some(2));
result<unsigned, std::string> y = failure("Nothing here");
assert(y.ok() == None);
```

**Remarks**

If self is rvalue and `T` is a reference type,
this function returns `boost::optional<dangling<std::reference_wrapper<std::remove_reference_t<T>>>>`.
