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
#include "err/error.hpp"

#include <bugspray/bugspray.hpp>

#include <type_traits>

using namespace err;

TEST_CASE("value-constructibility", "[error]")
{
    enum some_error
    {
        foo,
        bar,
        baz,
    };
    CHECK(std::is_constructible_v<error<foo>, some_error> && !std::is_convertible_v<some_error, error<foo>>);
    CHECK(std::is_constructible_v<error<bar>, some_error> && !std::is_convertible_v<some_error, error<bar>>);
    CHECK(std::is_constructible_v<error<baz>, some_error> && !std::is_convertible_v<some_error, error<baz>>);
    CHECK(std::is_constructible_v<error<foo, bar>, some_error> && !std::is_convertible_v<some_error, error<foo, bar>>);
    CHECK(std::is_constructible_v<error<foo, baz>, some_error> && !std::is_convertible_v<some_error, error<foo, baz>>);
    CHECK(std::is_constructible_v<error<bar, baz>, some_error> && !std::is_convertible_v<some_error, error<bar, baz>>);
    CHECK(std::is_constructible_v<error<foo, bar, baz>, some_error>
          && !std::is_convertible_v<some_error, error<foo, bar, baz>>);

    CHECK(error<foo>{foo} == foo);
    CHECK(error<bar>{bar} == bar);
    CHECK(error<baz>{baz} == baz);
    CHECK(error<foo, bar>{foo} == foo);
    CHECK(error<foo, bar>{bar} == bar);
    CHECK(error<foo, baz>{foo} == foo);
    CHECK(error<foo, baz>{baz} == baz);
    CHECK(error<bar, baz>{bar} == bar);
    CHECK(error<bar, baz>{baz} == baz);
    CHECK(error<foo, bar, baz>{foo} == foo);
    CHECK(error<foo, bar, baz>{bar} == bar);
    CHECK(error<foo, bar, baz>{baz} == baz);
}
EVAL_TEST_CASE("value-constructibility");