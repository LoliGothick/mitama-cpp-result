## Constructors

### default constructor [1/7]

```cpp
maybe() = default;
```

### copy constructor [2/7]

```cpp
maybe(maybe const&) = default;
```

### constructor [3/7]

```cpp
maybe(maybe&&) = default;
```

### constructor for nothing [4/7]

Delegates to default constructor.

```cpp
maybe(const nothing_t): maybe() {}
```

### constructor for maybe&lt;U&gt;, where constructible T from U  [5/7]

```cpp
template <typename U,
    std::enable_if_t<
        std::disjunction_v<
            mitamagic::is_pointer_like<std::remove_reference_t<U>>,
            std::is_pointer<std::remove_reference_t<U>>>,
    bool> = false>
maybe(U&& u) : storage_() {
    if (u) storage_.emplace(*std::forward<U>(u));
}
```

### in-place constructor [6/7]

```cpp
template <class... Args,
    std::enable_if_t<
        std::is_constructible_v<T, Args&&...>,
    bool> = false>
explicit maybe(std::in_place_t, Args&&... args)
    : storage_(etude::in_place(std::forward<Args>(args)...)) {}
```

### in-place constructor with initializer_list [7/7]

```cpp
template <class U, class... Args,
    std::enable_if_t<
        std::is_constructible_v<T, std::initializer_list<U>, Args&&...>,
    bool> = false>
explicit maybe(std::in_place_t, std::initializer_list<U> il, Args&&... args)
    : storage_(etude::in_place(il, std::forward<Args>(args)...)) {}
```

## Assignment operators

### copy assignment [1/2]

```cpp
maybe& operator=(maybe const&) = default;
```

### move assignment [1/2]

```cpp
maybe& operator=(maybe&&) = default;
```

