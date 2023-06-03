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

#ifndef ERR_JOIN_ARRAYS_HPP
#define ERR_JOIN_ARRAYS_HPP

#include <algorithm>
#include <array>
#include <concepts>

namespace err::detail
{
template<class... Arrs>
struct join_arrays;

template<class Arr>
struct join_arrays<Arr>
{
    consteval auto operator()(Arr arr) { return arr; }
};

template<class A1, class A2>
struct join_arrays<A1, A2>
{
    consteval auto operator()(A1 a1, A2 a2)
    {
        static_assert(std::same_as<typename A1::value_type, typename A2::value_type>);
        std::array<typename A1::value_type, a1.size() + a2.size()> result;
        std::ranges::copy(a1, result.begin());
        std::ranges::copy(a2, result.begin() + a1.size());
        return result;
    }
};

template<class A1, class A2, class... Tail>
struct join_arrays<A1, A2, Tail...>
{
    consteval auto operator()(A1 a1, A2 a2, Tail... tail)
    {
        auto res = join_arrays<A1, A2>{}(a1, a2);
        return join_arrays<decltype(res), Tail...>{}(res, tail...);
    }
};
} // namespace err::detail

#endif // ERR_JOIN_ARRAYS_HPP
