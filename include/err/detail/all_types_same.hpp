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

#ifndef ERR_ALL_TYPES_SAME_HPP
#define ERR_ALL_TYPES_SAME_HPP

#include <concepts>

namespace err::detail
{
template<typename... Ts>
struct all_types_same;

template<>
struct all_types_same<>
{
    static constexpr bool value = true;
};

template<typename T>
struct all_types_same<T>
{
    static constexpr bool value = true;
};

template<typename T, typename U>
struct all_types_same<T, U>
{
    static constexpr bool value = std::same_as<T, U>;
};

template<typename T, typename U, typename... Tail>
struct all_types_same<T, U, Tail...>
{
    static constexpr bool value = std::same_as<T, U> && all_types_same<Tail...>::value;
};

template<typename... Ts>
inline constexpr bool all_types_same_v = all_types_same<Ts...>::value;
} // namespace err::detail

#endif // ERR_ALL_TYPES_SAME_HPP
