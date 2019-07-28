
CHANGELOG
---------

**v5.0.0 (breaking change!)**

#**Types**

- `basic_result`
    - {`Result<T, E>` => `basic_result<mutability, T, E>`}
    - Enabled lvalue reference types for both of `T` and `E`
    - `result<T, E>` the alias template for immutable `basic_result`
    - `mut_result<T, E>` the alias template for mutable `basic_result`

- `success`
    - {`Ok<T>` => `success<T>`}
    - Enabled lvalue reference types for `T`.

- `failure`
    - {`Err<E>` => `failure<E>`}
    - Enabled lvalue reference types for `E`.

#**New APIs**

- `as_ref()`
- `as_mut()`
- `indirect()`
- `indirect_ok()`
- `indirect_err()`

**v1.2.0**

#**enhancement**

- new API: `transpose`
- new API: `map_or_else`

**v1.1.2**

#**bug fix**

- Result<T, E>::and_then(O&& op) do not perticipate in overload resolution if unless `op` returns `Result<U, E>`.

`std::invoke_result_t<O&&, T>` must be returns `Result<U, E>`.
It means `op` must only maps `T -> U`, never changes `E`.

- Result<T, E>::or_else(O&& op) do not perticipate in overload resolution if unless `op` returns `Result<T, F>`.

`std::invoke_result_t<O&&, E>` must be returns `Result<T, F>`.
It means `op` must only maps `E -> F`, never changes `T`.

**v1.1.1**

#**bug fix**

- Result<T, E>::unwrap_or_default() is now explicitly deleted if and only if `std::is_default_constructible_v<T> || std::is_aggregate_v<T>` is false.
