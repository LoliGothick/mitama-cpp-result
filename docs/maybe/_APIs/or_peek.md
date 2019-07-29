**or_peek(F f) -> maybe&lt;T&gt;**


```cpp
template <class F>
constexpr
std::enable_if_t<
    std::is_invocable_v<F>,
maybe&>
or_peek(F&& f) & ;

template <class F>
constexpr
std::enable_if_t<
    std::is_invocable_v<F>,
maybe const&>
or_peek(F&& f) const & ;

template <class F>
constexpr
std::enable_if_t<
    std::is_invocable_v<F>,
maybe&&>
or_peek(F&& f) && ;
```

Invokes the provided function and then return self (if nothing), or return self without doing anything (if any).

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
using namespace mitama;

int main() {
  maybe x = nothing;
  int hook = 0;
  assert(x.or_peek([&hook]{ hook = 42; }) == nothing);
  assert(hook == 42);
}
// end example
```
