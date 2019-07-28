**result&lt;T: DefaultConstructible, E&gt;::unwrap_or_default() -> T**

```cpp
auto basic_result<_, T, E>::unwrap_or_default() const &
  -> T ;
```

Returns the contained value or a default.

If `success`, returns the contained value, otherwise if `failure`, returns the default value for that type.

**Remarks**

This operator shall be defined as deleted unless `is_default_constructible_v<T>` is true.

**Example**

```cpp
auto good_year_from_input = "1909"s;
auto bad_year_from_input = "190blarg"s;
auto good_year = parse<int>(good_year_from_input).unwrap_or_default();
auto bad_year = parse<int>(bad_year_from_input).unwrap_or_default();
assert_eq(1909, good_year);
assert_eq(0, bad_year);   * 
```

