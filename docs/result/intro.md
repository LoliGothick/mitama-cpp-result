# Introduction

## Definition of class basic_result

```cpp
enum class mutability: bool {
    mut = false,
    immut = true,
};

template < mutability Mut >
inline constexpr bool is_mut_v = !static_cast<bool>(Mut);

template <mutability,
          class = std::monostate,   // success type
          class = std::monostate,   // failure type
          class = decltype(nullptr) // for detection idiom
>
class basic_result;
```

## Concepts

`basic_result<_, T, E>` is a class that holds either a success value type `T` or a failure value type `E`.
`basic_result<_, T, E>` holds values like `boost::variant<T, E>`.
Therefore, `T` and `E` must satisfy the following requirements for bounded types:

- CopyConstructible or MoveConstructible.
- Destructor upholds the no-throw exception-safety guarantee.
- Complete at the point of variant template instantiation.

In more detail, see [the document](https://www.boost.org/doc/libs/1_70_0/doc/html/variant/reference.html#variant.concepts).

## result/mut_result the alias templates

First (non-type) template parameter of `basic_result` is a value of enum class `mutability` for mutability control.

The library provides two type synonyms of `basic_result` as follows:

- `mut_result<T, E>` stands for `basic_result<mutability::mut, T, E>`
- `result<T, E>` stands for `basic_result<mutability::immut, T, E>`

You should use `mut_result<T, E>` if you want to resubstitute,
`result<T, E>` do not provides assignment operators or mutable accessors.

## success/failure the in-place factory classes

`success` and `failure` are in-place factory classes for `basic_result`.

If you want to initialize `result<T, E>` with successful value of `T`, initialize with `success<T>`.

```cpp
result<int, std::string> res = success(42);
```

Similarly, if you want to initialize `result<T, E>` with unsuccessful value of `E`, initialize with `failure<E>`.

```cpp
result<int, std::string> res = failure("error"s);
```

## Result of reference types

(In Progress...)
