**basic_result&lt;_, maybe&ltT&gt;, E&gt;::transpose() -> maybe&lt;basic_result&lt;_, T, E&gt;&gt;**

```cpp
template <mutability _mu, class T, class E>
class basic_result {
  auto basic_result<_, maybe<T>, E>::transpose()
    -> maybe<basic_result<_, T, E>> ;

  auto basic_result<_, maybe<T>, E>::transpose()
    -> maybe<basic_result<_, T, E>> ;
};
```

Transposes a `result` of an `maybe` into an `maybe` of a `result`.


`success(nothing)` will be mapped to `nothing`.
`success(just(v))` and `failure(v)` will be mapped to `just(success(v))` and `just(failure(v))`.

**Example**

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;

int main() {
  result<maybe<int>, std::string> x = success(just(5));
  maybe<result<int, std::string>> y = just(success(5));
  assert(x.transpose() == y);
}
// end example
```
