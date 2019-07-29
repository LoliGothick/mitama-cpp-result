**basic_result::and_peek(F f) -> basic_result**

```cpp
template <mutability _mu, class T, class E>
class basic_result {
  template <class F>
  constexpr
  std::enable_if_t<
    std::disjunction_v<
      std::is_invocable<F, T&>,
      std::is_invocable<F>>,
  basic_result&>
  and_peek(F&& f) & ;
  
  template <class F>
  std::enable_if_t<
    std::disjunction_v<
      std::is_invocable<F, T const&>,
      std::is_invocable<F>>,
  basic_result const&>
  and_peek(F&& f) const& ;
  
  template <class F>
  std::enable_if_t<
    std::disjunction_v<
      std::is_invocable<F, T&&>,
      std::is_invocable<F>>,
  basic_result&&>
  and_peek(F&& f) && ;
};
```

Peeks the contained success value and then returns self.

Invokes the provided function with the contained value and then return self (if success), or return self without doing anything (if failure).

**Example**

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
using namespace mitama;

int main() {
  result<int, std::string> x = success(42);
  int hook = 0;
  assert(x.and_peek([&](int const& v){ hook = v; }) == success(42));
  assert(hook == 42);
}
// end example
```
