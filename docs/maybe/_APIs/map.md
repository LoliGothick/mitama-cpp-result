**map(F f) -> maybe&lt;std::invoke_result_t&lt;F, T&gt;&gt;**

```cpp
template <class F,
    std::enable_if_t<
        std::is_invocable_v<F&&, T&>, bool> = false>
auto maybe<T>::map(F&& f) & -> maybe<std::invoke_result_t<F&&, T&>> ;

template <class F,
    std::enable_if_t<
        std::is_invocable_v<F&&, T const&>, bool> = false>
auto maybe<T>::map(F&& f) const& -> maybe<std::invoke_result_t<F&&, T const&>> ;

template <class F,
    std::enable_if_t<
        std::is_invocable_v<F&&, T&&>, bool> = false>
auto maybe<T>::map(F&& f) && -> maybe<std::invoke_result_t<F&&, T&&>> ;
```

Maps an `maybe<T>` to `maybe<U>` by applying a function to a contained value.

**Example**

```cpp
// begin example
#include <mitama/maybe/maybe.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  maybe maybe_some_string = just("Hello, World!"s);
  // `maybe::map` takes self *by ref*,
  // *not* consuming `maybe_some_string`
  maybe maybe_some_len = maybe_some_string.map(&std::string::size);

  assert(maybe_some_len == just(13u));
}
// end example
```
