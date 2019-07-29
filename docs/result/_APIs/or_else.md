**result&lt;T, E&gt;::or_else(O op) ->result&lt;T, F&gt;**
**where O: E -> result&lt;T, F&gt;**

```cpp
template <mutability _mu, class T, class E>
class basic_result {
  template <class O>
  constexpr auto basic_result<_, T, E>::or_else(O && op) const &
    -> std::invoke_result<O&&, E> ;
};
```

Calls `op` if the result is `failure`, otherwise returns the `success` value of self.

This function can be used for control flow based on result values.

**Remarks**

This constructor shall not participate in overload resolution unless `is_result_with_v<std::invoke_result_t<O, T>, success<T>>` is true.

**Example**

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  auto sq = [](unsigned x) -> result<unsigned, unsigned> { return success(x * x); };
  auto err = [](unsigned x) -> result<unsigned, unsigned> { return failure(x); };

  result<int, int> x = success(2u);
  result<int, int> y = failure(3u);

  assert(x.or_else(sq).or_else(sq) == success(2u));
  assert(x.or_else(err).or_else(sq) == success(2u));
  assert(y.or_else(sq).or_else(err) == success(9u));
  assert(y.or_else(err).or_else(err) == failure(3u));
}
// end example
```
