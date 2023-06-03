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

TEST_CASE("visit", "[error]")
{
    enum some_error
    {
        foo,
        bar,
        baz,
    };

    error<foo>           e;
    error<foo, bar, baz> ee{bar};

    CHECK(e.visit([](error<foo> e) { return 1; }) == 1);
    CHECK(ee.visit(overloaded{
              [](error<foo> e) { return 1; },
              [](error<bar> e) { return 2; },
              [](auto) { return 3; },
          })
          == 2);
    CHECK(ee.visit<double>(overloaded{
              [](error<foo> e) { return 42; },
              [](auto) { return 3.14159; },
          })
          == 3.14159);

    CHECK(visit(
              overloaded{
                  [](error<foo>, error<foo>) { return 1; },
                  [](error<foo>, error<bar>) { return 2; },
                  [](auto, auto) { return 3; },
              },
              e,
              ee)
          == 2);
}
EVAL_TEST_CASE("visit");