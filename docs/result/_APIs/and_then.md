**result&lt;T, E&gt;::and_then(O op) -> result&lt;U, E&gt;**
**where O: T -> result&lt;U, E&gt;**

```cpp
template <mutability _mu, class T, class E>
class basic_result {
  template <class O,
    std::enable_of>
  constexpr auto and_then(O&& op) &
    -> std::enable_if_t<is_convertible_result_with_v<std::invoke_result_t<O&&, T&>, failure<E>>,
    std::invoke_result_t<O, T>> ;

  template <class O>
  constexpr auto and_then(O&& op) const&
    -> std::enable_if_t<is_convertible_result_with_v<std::invoke_result_t<O&&, T const&>, failure<E>>,
    std::invoke_result_t<O, T>> ;

  template <class O>
  constexpr auto and_then(O&& op) &&
    -> std::enable_if_t<is_convertible_result_with_v<std::invoke_result_t<O&&, T&&>, failure<E>>,
    std::invoke_result_t<O, T>> ;
};
```

Calls `op` if the result is success, otherwise returns the failure value of self.

This function can be used for control flow based on result values.

**Remarks**

`and_then(O&&) qualifier` shall not participate in overload resolution unless `is_convertible_result_with_v<std::invoke_result_t<O&&, T qualifier>, failure<E>>` is true.

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

  assert(x.and_then(sq).and_then(sq) == success(16u));
  assert(x.and_then(sq).and_then(err) == failure(4u));
  assert(x.and_then(err).and_then(sq) == failure(2u));
  assert(y.and_then(sq).and_then(sq) == failure(3u));
}
// end example
```
