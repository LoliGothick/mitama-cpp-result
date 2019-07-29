**basic_result&lt;_, T, E&gt;::err() -> maybe&lt;E&gt;**

```cpp
template <class T, class E>
class basic_result<mutability::immut, T, E> {
  constexpr auto result<T, E>::err() &
    -> maybe<const E> ;

  constexpr auto result<T, E>::err() const &
    -> maybe<const E> ;

  constexpr auto result<T, E>::err() &&
    -> maybe<const E> ;
};

template <class T, class E>
class basic_result<mutability::mut, T, E> {
  constexpr auto mut_result<T, E>::err() &
    -> maybe<E> ;

  constexpr auto mut_result<T, E>::err() const &
    -> maybe<const E> ;

  constexpr auto mut_result<T, E>::err() &&
    -> maybe<E> ;
};
```

Converts from `basic_result` to `maybe`.

Converts self into an `maybe`, and discarding the success value, if any.

Note that these functions propagate mutability to element type of `maybe`.

**Remarks**

If self is rvalue and `E` is a reference type,
this function returns `maybe<dangling<std::reference_wrapper<std::remove_reference_t<E>>>>`.

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
  assert(x.err() == nothing);
  result<unsigned, std::string> y = failure("Nothing here"s);
  assert(y.err() == just("Nothing here"s));
}
// end example
```
