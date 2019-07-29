**basic_result&lt;_, T, E&gt;:::is_ok() -> bool**

```cpp
template <mutability _mu, class T, class E>
class basic_result {
  constexpr bool is_ok() const noexcept ;
};
```

Returns true if the result is `success`.

**Example**

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  result<unsigned, std::string> x = success(3);
  assert(x.is_ok() == true);
  result<unsigned, std::string> y = failure("Some error message"s);
  assert(y.is_ok() == false);
}
// end example
```
