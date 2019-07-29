**basic_result&lt;_, T, E&gt;::unwrap() -> E&**

```cpp
template <class T, class E>
class basic_result<mutability::immut, T, E> {
  auto unwrap() & -> const T ;

  auto unwrap() const& -> const T ;

  auto unwrap() && -> const T ;
};

template <class T, class E>
class basic_result<mutability::mut, T, E> {
  auto unwrap() & -> T ;

  auto unwrap() const& -> const T ;

  auto unwrap() && -> T ;
};
```

Unwraps a result, yielding the content of an `success`.

**Exception**

Raise `mitama::runtime_panic` if a result is containing `failure` value.

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
  {
    result<unsigned, std::string> x = success(2);
    assert(x.unwrap() == 2);
  }
  try {
    result<unsigned, std::string> x = failure("emergency failure"s);
    x.unwrap(); // panics with `emergency failure`
  }
  catch ( mitama::runtime_panic const& panic ) {
    std::cerr << panic.what() << std::endl;
  }
}
// end example
```
