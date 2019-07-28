# Introduction

## Definition of class maybe

```cpp
namespace mitama {
template <class T,
          class = void // for detection idiom
>
class maybe;
}
```

## Concepts

`maybe<T>` is a class that holds a value type `T` or nothing.
`maybe<T>` holds a value as `boost::optional<T>`.
Therefore, `T` must satisfy the following requirements for bounded types:

- Complete type

- Destructible

## just/nothing

`just` and `nothing` are in-place factory for `maybe<T>`.

If you want to initialize `maybe<T>` with some value, initialize with `just(value)`.

```cpp
mitama::maybe<int> some_int = mitama::just(42);
```

Similarly, if you want to initialize `maybe<T>` with none value, initialize with `nothing`.

```cpp
mitama::maybe<int> nope_int = mitama::nothing;
```

