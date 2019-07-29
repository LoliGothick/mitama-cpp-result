**basic_result&lt;_, T, E&gt;::map_err(O op) -> basic_result&lt;_, T, F&gt;**
**where O: E -> F**

```cpp
template <mutability _mu, class T, class E>
class basic_result {
template <class O>
constexpr auto map_err(O && op) const &
  -> std::enable_if_t<std::is_invocable_v<O, E>, result<T, std::invoke_result_t<O, E>>> ;
};
```

Maps a `result<T, E>` to `result<T, F>` by applying a function to a contained failure value, leaving an success value untouched.

This function can be used to pass through a successful result while handling an error.

**Example**

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  auto stringify = [](unsigned x) -> std::string{
    return "error code: "s + std::to_string(x);
  };
  result<unsigned, unsigned> x = success(2);
  assert(x.map_err(stringify) == success(2u));
  result<unsigned, unsigned> y = failure(13);
  assert(y.map_err(stringify) == failure("error code: 13"s));
}
// end example
```
