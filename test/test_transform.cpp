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

using namespace err;

TEST_CASE("transform", "[error]")
{
    enum some_error
    {
        foo,
        bar,
        baz,
        bam,
        bom,
    };

    error<foo>           e;
    error<foo, bar, baz> ee{foo};

    CHECK(e.transform([](error<foo> e) { return error<bar>{}; }) == bar);
    CHECK(ee.transform(overloaded{
              [](error<foo> e) { return error<bar>{}; },
              [](error<baz> e) { return error<bom>{}; },
              [](auto e) { return e; },
          })
          == bar);
    CHECK(ee.transform(
              [](error<foo, bar, baz> e) -> error<bar, baz, bam>
              {
                  if (e == foo)
                      return error<bam>{};
                  return error<bar, baz, bam>(e);
              })
          == bam);
}
EVAL_TEST_CASE("transform");