**basic_result&lt;_, T, E&gt;::unwrap_err() -> E&**

```cpp
template <class T, class E>
class basic_result<mutability::immut, T, E> {
  auto unwrap_err() & -> const E ;

  auto unwrap_err() const& -> const E ;

  auto unwrap_err() && -> const E ;
};

template <class T, class E>
class basic_result<mutability::mut, T, E> {
  auto mut_unwrap_err() & -> E ;

  auto mut_unwrap_err() const& -> const E ;

  auto mut_unwrap_err() && -> E ;
};
```

Unwraps a result, yielding the content of an `failure`.

**Exception**

Raise `mitama::runtime_panic` if a result is containing `success` value.

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
  try {
    result<unsigned, std::string> x = success(2);
    x.unwrap_err(); // panics with `2`
  }
  catch (runtime_panic const &panic)
  {
    std::cerr << panic.what() << std::endl;
  }
  {
    result<unsigned, std::string> x = failure("emergency failure"s);
    assert(x.unwrap_err() == "emergency failure"s);
  }
}
// end example
```
