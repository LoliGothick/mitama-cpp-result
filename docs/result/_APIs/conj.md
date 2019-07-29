**result&ltT, E&gt;::conj(result&ltU, E&gt; res) -> result&ltU, E&gt;**

```cpp
template <mutability _mu, class T, class E>
class basic_result {
  template <mutability _, class U>
  constexpr auto basic_result<_, T, E>::conj(basic_result<_, U, E> const& res) const &
    -> basic_result<U, E> ;

  template <mutability _, class U>
  constexpr auto basic_result<_, T, E>::operator&&(basic_result<_, U, E> const & res) const &
    -> basic_result<U, E> ;
};
```

Returns `res` if the result is `success`, otherwise returns the `failure` value of self.

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
    result<std::string, std::string> y = failure("late error"s);
    assert((x && y) == failure("late error"s));
  }
  {
    result<unsigned, std::string> x = failure("early error"s);
    result<std::string, std::string> y = success("foo"s);
    assert((x && y) == failure("early error"s));
  }
  {
    result<unsigned, std::string> x = failure("not a 2"s);
    result<std::string, std::string> y = failure("late error"s);
    assert((x && y) == failure("not a 2"s));
  }
  {
    result<unsigned, std::string> x = success(2);
    result<std::string, std::string> y = success("different result type"s);
    assert((x && y) == success("different result type"s));
  }
}
// end example
```
