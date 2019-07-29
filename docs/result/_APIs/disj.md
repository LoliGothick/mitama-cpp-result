**basic_result&lt;_, T, E&gt;::disj(basic_result&lt;_, T, F&gt; res) -> basic_result&lt;_, T, F&gt;**

```cpp
template <mutability _mu, class T, class E>
class basic_result {
  template <class F>
  constexpr auto basic_result<_, T, E>::disj(basic_result<_, T, F> const& res) const &
    -> basic_result<_, T, F> ;

  template <class F>
  constexpr auto basic_result<_, T, E>::operator||(basic_result<_, T, F> const& res) const &
    -> basic_result<_, T, F> ;
};
```

Returns `res` if the result is `failure`, otherwise returns the `success` value of self.

Arguments passed to or are eagerly evaluated; if you are passing the result of a function call, it is recommended to use `or_else`, which is lazily evaluated.

**Example**

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <cassert>
#include <string>
using namespace mitama;
using namespace std::string_literals;

int main() {
  {
    result<unsigned, std::string> x = success(2);
    result<unsigned, std::string> y = failure("late error"s);
    assert(x.disj(y) ==  success(2u));
    assert((x || y) ==  success(2u));
  }
  {
    result<unsigned, std::string> x = failure("early error"s);
    result<unsigned, std::string> y = success(2);
    assert(x.disj(y) ==  success(2u));
    assert((x || y) ==  success(2u));
  }
  {
    result<unsigned, std::string> x = failure("not a 2"s);
    result<unsigned, std::string> y = failure("late error"s);
    assert(x.disj(y) ==  failure("late error"s));
    assert((x || y) ==  failure("late error"s));
  }
  {
    result<unsigned, std::string> x = success(2);
    result<unsigned, std::string> y = success(100);
    assert(x.disj(y) ==  success(2u));
    assert((x || y) ==  success(2u));
  }
}
// end example
```

