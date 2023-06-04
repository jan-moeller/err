//
// MIT License
//
// Copyright (c) 2023 Jan Möller
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef ERR_ERROR_IMPL_HPP
#define ERR_ERROR_IMPL_HPP

#include "err/detail/all_types_same.hpp"
#include "err/detail/apply_non_type_template_arg.hpp"
#include "err/detail/first_non_type_template_arg.hpp"
#include "err/detail/forward_like.hpp"
#include "err/detail/is_overlap.hpp"
#include "err/detail/is_subset.hpp"
#include "err/detail/join_arrays.hpp"

#include <ctrx/contracts.hpp>

#include <array>
#include <expected>
#include <functional>
#include <utility>
#include <variant>

namespace err::detail
{
template<auto... Enumerators>
    requires(sizeof...(Enumerators) > 0)
            && detail::all_types_same_v<decltype(Enumerators)...> && (std::is_enum_v<decltype(Enumerators)> && ...)
class error_impl;

template<class Visitor, auto... Es>
constexpr auto transform(Visitor&& vis, error_impl<Es...> e) -> decltype(auto);

template<class Visitor, class... Errors>
constexpr auto visit(Visitor&& vis, Errors&&... errors) -> decltype(auto);

template<typename R, class Visitor, class... Errors>
constexpr auto visit(Visitor&& vis, Errors&&... errors) -> decltype(auto);

template<auto... Enumerators>
    requires(sizeof...(Enumerators) > 0)
            && detail::all_types_same_v<decltype(Enumerators)...> && (std::is_enum_v<decltype(Enumerators)> && ...)
class error_impl
{
  public:
    using value_type = std::remove_cvref_t<decltype(detail::first_non_type_template_arg<Enumerators...>())>;
    static constexpr auto possible_values = std::array{Enumerators...};

    constexpr error_impl()
        requires(sizeof...(Enumerators) != 1)
    = delete;

    constexpr error_impl() noexcept
        requires(sizeof...(Enumerators) == 1)
        : m_value(detail::first_non_type_template_arg<Enumerators...>())
    {
    }

    constexpr explicit error_impl(value_type other)
        : m_value(other)
    {
        CTRX_PRECONDITION(((other == Enumerators) || ...));
    }

    template<value_type... Es>
        requires(detail::is_overlap<value_type>({Es...}, {Enumerators...}))
    constexpr explicit(!detail::is_subset<value_type>({Es...}, {Enumerators...}))
        error_impl(error_impl<Es...> other) noexcept(detail::is_subset<value_type>({Es...}, {Enumerators...}))
        : m_value(static_cast<value_type>(other))
    {
        CTRX_PRECONDITION(((static_cast<value_type>(other) == Enumerators) || ...));
    }

    template<value_type... Es>
        requires(detail::is_subset<value_type>({Es...}, {Enumerators...}))
    constexpr auto operator=(error_impl<Es...> other) noexcept -> error_impl&
    {
        m_value = static_cast<value_type>(other);
        return *this;
    }

    template<value_type... Es>
        requires(detail::is_overlap<value_type>({Es...}, {Enumerators...}))
    constexpr auto operator==(error_impl<Es...> other) const noexcept -> bool
    {
        return m_value == static_cast<value_type>(other);
    }
    constexpr auto operator==(value_type other) const noexcept -> bool { return m_value == other; }

    constexpr explicit operator value_type() const noexcept { return m_value; }

    template<typename T, auto... Es>
        requires(detail::is_overlap<value_type>({Es...}, {Enumerators...}))
    constexpr explicit(!detail::is_subset({Enumerators...}, {Es...}))
    operator std::expected<T, error_impl<Es...>>() const
    {
        return std::expected<T, error_impl<Es...>>{std::unexpect, static_cast<error_impl<Es...>>(*this)};
    }

    template<class Visitor>
    constexpr auto transform(Visitor&& vis) const -> decltype(auto)
    {
        return detail::transform(std::forward<Visitor>(vis), *this);
    }

    template<class Visitor>
    constexpr auto visit(Visitor&& vis) const -> decltype(auto)
    {
        return detail::visit(std::forward<Visitor>(vis), *this);
    }

    template<typename R, class Visitor>
    constexpr auto visit(Visitor&& vis) const -> decltype(auto)
    {
        return detail::visit<R>(std::forward<Visitor>(vis), *this);
    }

  private:
    value_type m_value;
};

template<auto... Es>
constexpr auto to_variant(error_impl<Es...> e) -> std::variant<error_impl<Es>...>
{
    std::variant<error_impl<Es>...> var{};
    using value_type = error_impl<Es...>::value_type;
    auto fn          = [&e, &var]<value_type E>()
    {
        if (static_cast<value_type>(e) == E)
            var = error_impl<E>{};
    };
    (fn.template operator()<Es>(), ...);
    return var;
};

template<typename... Ts>
struct combined_error
{
    consteval static auto remove_duplicates_and_sort()
    {
        constexpr auto result = []()
        {
            std::array a = join_arrays<decltype(Ts::possible_values)...>{}(Ts::possible_values...);
            std::ranges::sort(a);
            auto remove_range = std::ranges::unique(a);
            return std::make_pair(a, std::distance(a.begin(), remove_range.begin()));
        }();
        std::array<typename std::tuple_element<0, decltype(result)>::type::value_type, result.second> r;
        std::copy(result.first.begin(), result.first.begin() + result.second, r.begin());
        return r;
    }
    using type = detail::apply_non_type_template_arg_t<error_impl, remove_duplicates_and_sort()>;
};

template<class Visitor, class... Errors>
constexpr auto visit(Visitor&& vis, Errors&&... errors) -> decltype(auto)
{
    return std::visit(std::forward<Visitor>(vis), to_variant(errors)...);
}

template<typename R, class Visitor, class... Errors>
constexpr auto visit(Visitor&& vis, Errors&&... errors) -> decltype(auto)
{
    return std::visit<R>(std::forward<Visitor>(vis), to_variant(errors)...);
}

template<class Visitor, auto... Es>
constexpr auto transform(Visitor&& vis, error_impl<Es...> e) -> decltype(auto)
{
    using expected_result = typename detail::combined_error<std::invoke_result_t<Visitor, error_impl<Es>>...>::type;
    return visit<expected_result>(std::forward<Visitor>(vis), e);
}

template<class Visitor, typename T, auto... Es>
constexpr auto transform_error(Visitor&& vis, std::expected<T, error_impl<Es...>> const& e) -> decltype(auto)
{
    return e.transform_error([v = std::forward<Visitor>(vis)](error_impl<Es...> err)
                             { return err.transform(forward_like<Visitor>(v)); });
}

} // namespace err::detail

#endif // ERR_ERROR_IMPL_HPP
