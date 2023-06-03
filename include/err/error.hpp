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

#ifndef ERR_ERROR_HPP
#define ERR_ERROR_HPP

#include "err/detail/all_types_same.hpp"
#include "err/detail/error_impl.hpp"

#include <type_traits>

namespace err
{
template<auto... Enumerators>
    requires detail::all_types_same_v<decltype(Enumerators)...> && (std::is_enum_v<decltype(Enumerators)> && ...)
using error = detail::combined_error<detail::error_impl<Enumerators>...>::type;

using detail::transform;
using detail::transform_error;
using detail::visit;
} // namespace err

namespace std
{
template<auto... As, auto... Bs>
struct common_type<err::detail::error_impl<As...>, err::detail::error_impl<Bs...>>
{
    using type = err::detail::combined_error<err::detail::error_impl<As...>, err::detail::error_impl<Bs...>>::type;
};
} // namespace std

#endif // ERR_ERROR_HPP
