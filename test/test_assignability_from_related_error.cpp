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

TEST_CASE("assignability from related error", "[error]")
{
    enum some_error
    {
        foo,
        bar,
        baz,
    };
    CHECK(std::is_assignable_v<error<foo>, error<foo>>);
    CHECK(!std::is_assignable_v<error<foo>, error<bar>>);
    CHECK(!std::is_assignable_v<error<foo>, error<baz>>);

    CHECK(std::is_assignable_v<error<bar>, error<bar>>);
    CHECK(!std::is_assignable_v<error<bar>, error<foo>>);
    CHECK(!std::is_assignable_v<error<bar>, error<baz>>);

    CHECK(std::is_assignable_v<error<baz>, error<baz>>);
    CHECK(!std::is_assignable_v<error<baz>, error<foo>>);
    CHECK(!std::is_assignable_v<error<baz>, error<bar>>);

    CHECK(std::is_assignable_v<error<foo, bar>, error<foo>>);
    CHECK(std::is_assignable_v<error<foo, bar>, error<bar>>);
    CHECK(std::is_assignable_v<error<foo, bar>, error<foo, bar>>);
    CHECK(!std::is_assignable_v<error<foo, bar>, error<foo, bar, baz>>);
    CHECK(!std::is_assignable_v<error<foo, bar>, error<baz>>);

    CHECK(std::is_assignable_v<error<foo, baz>, error<foo>>);
    CHECK(std::is_assignable_v<error<foo, baz>, error<baz>>);
    CHECK(std::is_assignable_v<error<foo, baz>, error<foo, baz>>);
    CHECK(!std::is_assignable_v<error<foo, baz>, error<foo, bar, baz>>);
    CHECK(!std::is_assignable_v<error<foo, baz>, error<bar>>);

    CHECK(std::is_assignable_v<error<bar, baz>, error<bar>>);
    CHECK(std::is_assignable_v<error<bar, baz>, error<baz>>);
    CHECK(std::is_assignable_v<error<bar, baz>, error<bar, baz>>);
    CHECK(!std::is_assignable_v<error<bar, baz>, error<foo, bar, baz>>);
    CHECK(!std::is_assignable_v<error<bar, baz>, error<foo>>);

    CHECK(std::is_assignable_v<error<foo, bar, baz>, error<foo>>);
    CHECK(std::is_assignable_v<error<foo, bar, baz>, error<bar>>);
    CHECK(std::is_assignable_v<error<foo, bar, baz>, error<baz>>);
    CHECK(std::is_assignable_v<error<foo, bar, baz>, error<bar, baz>>);
    CHECK(std::is_assignable_v<error<foo, bar, baz>, error<foo, bar, baz>>);

    {
        error<foo> e{foo};
        e = error<foo>{};
        CHECK(e == foo);
    }
    {
        error<foo, bar> e{foo};
        e = error<bar>{};
        CHECK(e == bar);
    }
    {
        error<foo, bar, baz> e{foo};
        e = error<bar, baz>{bar};
        CHECK(e == bar);
    }
}
EVAL_TEST_CASE("assignability from related error");