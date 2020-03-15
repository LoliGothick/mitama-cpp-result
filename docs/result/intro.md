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

Both of `T` and `E` must satisfy the following requirements:

- Complete type (if it is not reference type) or Reference Type (possibly incomplete type)
- Destructor upholds the no-throw exception-safety guarantee.

## result/mut_result the alias templates

First (non-type) template parameter of `basic_result` is a value of enum class `mutability` for mutability control.

The library provides two type synonyms of `basic_result` as follows:

- `mut_result<T, E>` stands for `basic_result<mutability::mut, T, E>`
- `result<T, E>` stands for `basic_result<mutability::immut, T, E>`

You should use `mut_result<T, E>` if you want to resubstitute,
`result<T, E>` do not provides assignment operators or mutable accessors.

## success/failure the in-place factory classes

`success_t` and `failure_t` are in-place factory classes for `basic_result`.

If you want to initialize `result<T, E>` with successful value of `T`, initialize with `success_t<T>`.

```cpp
result<int, std::string> res = success_t{42};
```

Similarly, if you want to initialize `result<T, E>` with unsuccessful value of `E`, initialize with `failure_t<E>`.

```cpp
result<int, std::string> res = failure_t{"error"};
```

However, using `success` and `failure` (factory methods) is recommended.

```cpp
result<int, std::string> ok = success(42);
result<int, std::string> ng = failure("error");
```

## Result with reference types

### Using reference type

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <string>
#include <cassert>
using namespace mitama;

int main() {
    int i = 1;
    mut_result<int&, std::string&> res = success(i);
    res.unwrap() = 2; // get reference to `i`, and assign `2`.

    assert(i == 2);
}
// end example
```

### Using reference of imcomplete type [since v8.0.0]

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <string>
#include <type_traits>
#include <cassert>

struct incomplete_type;
incomplete_type& get_incomplete_type();
template <class T, class=void> struct is_complete_type: std::false_type {};
template <class T> struct is_complete_type<T, std::void_t<decltype(sizeof(T))>>: std::true_type {};

using namespace mitama;

int main() {
  static_assert(!is_complete_type<incomplete_type>::value);
  [[maybe_unused]]
  result<incomplete_type&> res = success<incomplete_type&>(get_incomplete_type()); // use incomplete_type& for result
}

struct incomplete_type {};

incomplete_type& get_incomplete_type() {
  static incomplete_type obj = incomplete_type{};
  return obj;
}
// end example
```
