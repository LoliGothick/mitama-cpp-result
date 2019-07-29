**basic_result&lt;_, T, E&gt;::map(O op) -> basic_result&lt;_, U, E&gt;**
**where O: T -> U**

```cpp
template <mutability _mu, class T, class E>
class basic_result {
constexpr auto map(O && op)const &
  -> std::enable_if_t<std::is_invocable_v<O, T>, result<std::invoke_result_t<O, T>, E>> ;
};
```

Maps a `result<T, E>` to `result<U, E>` by applying a function to a contained `success` value, leaving an `failure` value untouched.

This function can be used to compose the results of two functions.

**Example**

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
#include <boost/lambda/lambda.hpp>
using namespace mitama;
using namespace std::string_literals;
using boost::lambda::_1;

int main() {
  result<int, int> ok = success(2);
  assert(ok.map(_1 * 2) == success(4));

  result<int, int> err = failure(2);
  assert(err.map(_1 * 2) == failure(2));
}
// end example
```
