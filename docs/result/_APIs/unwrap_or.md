**basic_result&lt;_, T, E&gt;::unwrap_or(T const& optb) -> T**

```cpp
template <mutability _mu, class T, class E>
class basic_result {
  auto basic_result<_, T, E>::unwrap_or(T const& optb) const noexcept
};
```

Unwraps a result, yielding the content of an `success`. Else, it returns `optb`.

Arguments passed to unwrap_or are eagerly evaluated; if you are passing the result of a function call, it is recommended to use `unwrap_or_else`, which is lazily evaluated.

**Example**

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  result<unsigned, unsigned> ok = success(2);
  result<unsigned, unsigned> err = failure(2);
  assert(ok.unwrap_or(1u) == 2u);
  assert(err.unwrap_or(1u) == 1u);
}
// end example
```
