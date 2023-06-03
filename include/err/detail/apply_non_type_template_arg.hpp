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

#ifndef ERR_APPLY_NON_TYPE_TEMPLATE_ARG_HPP
#define ERR_APPLY_NON_TYPE_TEMPLATE_ARG_HPP

#include <utility>

#include <cstddef>

namespace err::detail
{
template<template<auto...> typename T, auto Arr, typename>
struct apply_non_type_template_arg;

template<template<auto...> typename T, auto Arr, std::size_t... Is>
struct apply_non_type_template_arg<T, Arr, std::index_sequence<Is...>>
{
    using type = T<Arr[Is]...>;
};

template<template<auto...> typename T, auto Arr>
using apply_non_type_template_arg_t = apply_non_type_template_arg<T, Arr, std::make_index_sequence<Arr.size()>>::type;

} // namespace err::detail

#endif // ERR_APPLY_NON_TYPE_TEMPLATE_ARG_HPP
