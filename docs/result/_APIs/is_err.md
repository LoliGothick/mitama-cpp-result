**basic_result&lt;_, T, E&gt;::is_err() -> bool**

```cpp
template <mutability _mu, class T, class E>
class basic_result {
  constexpr bool is_err() const noexcept ;
};
```

Returns true if the result is failure.

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
  assert(x.is_err() == false);
  result<unsigned, std::string> y = failure("Some error message"s);
  assert(y.is_err() == true);
}
// end example
```
