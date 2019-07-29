**basic_result&lt;_, T, E&gt;::unwrap_or_else() -> T**

```cpp
template <mutability _mu, class T, class E>
class basic_result {
  template <class O>
  auto basic_result<_, T, E>::unwrap_or_else(O && op) const noexcept
    -> std::enable_if_t<std::is_invocable_r_v<T, O, E>, T> ;
};
```

Unwraps a result, yielding the content of an `success`. If the value is an `failure` then it calls `op` with its value.

**Example**

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  result<int, std::string> ok = success(2);
  result<int, std::string> err = failure("foo"s);
  assert(ok.unwrap_or_else(&std::string::size) == 2);
  assert(err.unwrap_or_else(&std::string::size) == 3);
}
// end example
```
