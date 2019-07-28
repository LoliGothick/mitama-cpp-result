**result&ltT, E&gt;::disj(result&ltT, F&gt; res) -> result&ltT, F&gt;**

```cpp
template <class F>
constexpr auto basic_result<_, T, E>::disj(basic_result<_, T, F> const& res) const &
  -> basic_result<_, T, F> ;

template <class F>
constexpr auto basic_result<_, T, E>::operator||(basic_result<_, T, F> const& res) const &
  -> basic_result<_, T, F> ;
```

Returns `res` if the result is `failure`, otherwise returns the `success` value of self.

Arguments passed to or are eagerly evaluated; if you are passing the result of a function call, it is recommended to use `or_else`, which is lazily evaluated.

**Example**

```cpp
{
  result<unsigned, std::string> x = success(2);
  result<unsigned, std::string> y = failure("late error"s);
  assert_eq(x || y, success(2u));
  assert_eq(x.disj(y), success(2u));
}
{
  result<unsigned, std::string> x = failure("early error"s);
  result<unsigned, std::string> y = success(2);
  assert_eq(x || y, success(2u));
  assert_eq(x.disj(y), success(2u));
}
{
  result<unsigned, std::string> x = failure("not a 2"s);
  result<unsigned, std::string> y = failure("late error"s);
  assert_eq(x || y, failure("late error"s));
  assert_eq(x.disj(y), failure("late error"s));
}
{
  result<unsigned, std::string> x = success(2);
  result<unsigned, std::string> y = success(100);
  assert_eq(x || y, success(2u));
  assert_eq(x.disj(y), success(2u));
}
```

