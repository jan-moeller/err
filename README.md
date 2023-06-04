# err

[![GCC](https://github.com/jan-moeller/err/actions/workflows/gcc.yml/badge.svg)](https://github.com/jan-moeller/err/actions/workflows/gcc.yml)

Err is a tiny library providing `err::error`, a class template to use with error
enumerations and `std::expected`.

It allows you to define all errors in a central `enum` (or several thereof), but
still annotate your functions with which of the enumerators they may return in
the error case. While the errors can be checked with a traditional if-else
cascade, `err::error` also provides a variant-like interface that allows you to
write code that fails to compile if the checks are not exhaustive.

For example:

```c++
enum file_error // Our file error type
{
    permission_error,
    write_error,
    read_error,
};

enum system_error // more general type
{
    generic_error,
    wrong_permission,
    cat_error,
};

// On error, only ever returns read_error or permission_error
auto read_file(std::string_view file)
    -> std::expected<string, error<read_error, permission_error>>;

// Errors are from system_error type, function needs to translate
auto read_config() -> std::expected<config, error<generic_error, wrong_permission>>
{
    auto f = read_file("~/config.conf");
    if (f)
        return parse_config(*f);
    
    // In the bad case, transform errors to system_error type
    return transform_error(overloaded{
        [](error<permission_error>){ return error<wrong_permission>{}; },
        [](auto){ return error<generic_error>{}; }, // anything else is generic
    }, f);
}
```

## Synopsis

```c++
namespace err
{
template<auto... Enumerators>
    requires /* see below */
struct error
{
    using value_type = /* see below */;
    static constexpr auto possible_values = std::array{Enumerators...};
    
    constexpr error() noexcept
        requires (sizeof...(Enumerators) == 1);
    
    constexpr explicit error(value_type other);
    
    template<value_type... Es>
        requires /* see below */
    constexpr explicit(/* see below */) error(error<Es...> other) noexcept(/* see below */);
    
    template<value_type... Es>
        requires /* see below */
    constexpr auto operator=(error<Es...> other) noexcept -> error&;
    
    template<value_type... Es>
        requires /* see below */
    constexpr auto operator==(error<Es...> other) const noexcept -> bool;
    constexpr auto operator==(value_type other) const noexcept -> bool;
    
    constexpr explicit operator value_type() const noexcept;
    
    template<typename T, auto... Es>
        requires /* see below */
    constexpr explicit(/* see below */) operator std::expected<T, error<Es...>>() const;

    template<class Visitor>
    constexpr auto visit(Visitor&& vis) const -> decltype(auto);
    
    template<typename R, class Visitor>
    constexpr auto visit(Visitor&& vis) const -> R;
    
    template<class Visitor>
    constexpr auto transform(Visitor&& vis) const -> decltype(auto);
};

template<class Visitor, class... Errors>
constexpr auto visit(Visitor&& vis, Errors&&... errors) -> decltype(auto);

template<typename R, class Visitor, class... Errors>
constexpr auto visit(Visitor&& vis, Errors&&... errors) -> R;

template<class Visitor, auto... Es>
constexpr auto transform(Visitor&& vis, error<Es...> e) -> decltype(auto);

template<class Visitor, typename T, auto... Es>
constexpr auto transform_error(Visitor&& vis, std::expected<T, error<Es...>> const& e) -> decltype(auto);
} // namespace err

namespace std
{
template<auto... As, auto... Bs>
struct common_type<err::error<As...>, err::error<Bs...>>;
} // namespace std
```

## Reference

`error` can be instantiated with any number of enumerators of the same type.
The order in which enumerators are listed in the template argument list is
irrelevant; `error<a, b>` is the same type as `error<b, a>`. The number of
times an enumerator is mentioned in the template argument list is irrelevant
as well; `error<a, a>` is the same type as `error<a>`.

### Related `error` Types

Two instantiations `error<As..>` and `error<Bs...>` are called *related* if
their lists of `Enumerators` overlap; i.e. if some elements of `Bs` also appear
in `As`.

`error<Bs...>` is called *closely related* to `error<As...>` if `Bs` is a
strict subset of `As`; i.e. if all elements of `Bs` also appear in `As`.

### Member Types

`value_type` is the common type of all `Enumerators`.

### Static Member Objects

`possible_values` is an array listing all `Enumerators`, de-duplicated and
potentially in a different order than listed in the template argument list.

### Default-Constructor

`error` is default-constructible only if it can contain only a single possible
value (i.e. `possible_values.size() == 1`). In this case, the default-constructor
constructs a new `error` containing the only possible value.

### Construction From `value_type`

`error` is explicitly constructible from `value_type`. If the value used to call
this constructor is not in the list of `possible_values`, behavior is undefined.

### Construction & Assignment From Related `error`s

`error<As...>` is constructible from `error<Bs...>`, as long as `As` and `Bs`
are *related*. Such construction is implicit only from *closely related*
`error`s. Otherwise, it is explicit.

In the case of an explicit construction, if the `error` used to call this
constructor contains a value not in the list of `possible_values`, behavior is
undefined.

The constructor is `noexcept(true)` iff it is not `explicit`. Otherwise, it is
`noexcept(false)`.

Direct assignment is provided from *closely related* `error`s only.

### Equality Comparison

`error` is equality-comparable to its `value_type`. `error` is also
equality-comparable to related `error`s, as long as the types are *related*.

### Conversion

`error` is explicitly convertible to its underlying `value_type`.

It is further convertible to `std::expected`, as long as it uses a related
`error` as its `unexpected` type. If the `unexpected` type is *closely related*,
the conversion is implicit.

### Visitation

`error`s can be visited by calling the free function `visit`. This function
behaves as if calling `std::visit` on a `std::variant<error<Es>...>`, where
`Es` are all `possible_values`.

The member function `visit` behaves analogous, except it doesn't offer multi
visitation.

### Transformation

`error`s can be transformed to `error`s of different type by using the free
function `transform`. This function behaves as if calling `std::visit` on a
`std::variant<error<Es>...>`, where `Es` are all `possible_values`. The visitor
must return `error`s, but they may be unrelated to the original `error`.

The member function `transform` behaves analogous.

The `transform_error` free function can be used to transform a
`std::expected<T, error<As...>` to a `std::expected<T, error<Bs...>`, where
`Bs` can be entirely unrelated to `As`. It calls the visitor only if the
`expected` contains an `unexpected` value; otherwise, it uses the visitor only
to deduce the new type, and then returns the `expected` value as-is.

### `std::common_type`

`common_type` is specialized to provide the `error` type with the least number
of `possible values`, such that all arguments are *closely related* to that type.

### `overloaded`

This library also provides an implementation of `overloaded` in a separate
header `err/overloaded.hpp`. This is a convenience type to allow overloading
lambdas as visitors. Its implementation is identical to what's suggested on
[cppreference](https://en.cppreference.com/w/cpp/utility/variant/visit).
