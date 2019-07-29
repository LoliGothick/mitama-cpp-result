**basic_result&lt;_, T, E&gt;::ok() -> maybe&lt;T&gt;**

```cpp
template <class T, class E>
class basic_result<mutability::immut, T, E> {
  constexpr auto result<T, E>::ok() &
    -> maybe<const T> ;

  constexpr auto result<T, E>::ok() const &
    -> maybe<const T> ;

  constexpr auto result<T, E>::ok() &&
    -> maybe<const T> ;
};

template <class T, class E>
class basic_result<mutability::mut, T, E> {
  constexpr auto mut_result<T, E>::ok() &
    -> maybe<T> ;

  constexpr auto mut_result<T, E>::ok() const &
    -> maybe<const T> ;

  constexpr auto mut_result<T, E>::ok() &&
    -> maybe<T> ;
};
```

Converts from `basic_result` to `maybe`.

Converts self into an `maybe`, and discarding the failure value, if any.

Note that these functions propagate mutability to element type of `maybe`.

**Remarks**

If self is rvalue and `T` is a reference type,
this function returns `maybe<dangling<std::reference_wrapper<std::remove_reference_t<T>>>>`.

**Example**

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  result<unsigned, std::string> x = success(2);
  assert(x.ok() == just(2));
  result<unsigned, std::string> y = failure("Nothing here"s);
  assert(y.ok() == nothing);
}
// end example
```
