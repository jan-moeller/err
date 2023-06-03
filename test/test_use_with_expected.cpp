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
#include "err/overloaded.hpp"

#include <bugspray/bugspray.hpp>
#include <expected>

using namespace err;

enum some_error
{
    foo,
    bar,
};

enum other_error
{
    baz,
    bam,
    bom,
};

constexpr auto do_the_foo(int i) -> std::expected<int, error<foo, bar>>
{
    if (i > 0)
        return i;
    if (i == 0)
        return error<foo>{};
    return error<bar>{};
}

constexpr auto do_the_bar(int i) -> std::expected<int, error<baz, bam, bom>>
{
    if (i % 2 != 0)
        return error<bom>{};
    auto const r = do_the_foo(i);
    return transform_error(
        overloaded{
            [](error<foo>) { return error<bam>{}; },
            [](error<bar>) { return error<baz>{}; },
        },
        r);
}

constexpr auto do_the_baz(int i) -> std::expected<int, error<baz, bam, bom>>
{
    auto r = do_the_bar(i);
    return r.transform([](int j) { return j * 2; });
}

TEST_CASE("use with expected", "[error]")
{
    CHECK(do_the_bar(2) == 2);
    CHECK(do_the_bar(1) == std::unexpected{error<bom>{}});
    CHECK(do_the_bar(0) == std::unexpected{error<bam>{}});
    CHECK(do_the_bar(-2) == std::unexpected{error<baz>{}});

    CHECK(do_the_baz(2) == 4);
    CHECK(do_the_baz(1) == std::unexpected{error<bom>{}});
    CHECK(do_the_baz(0) == std::unexpected{error<bam>{}});
    CHECK(do_the_baz(-2) == std::unexpected{error<baz>{}});
}
EVAL_TEST_CASE("use with expected");