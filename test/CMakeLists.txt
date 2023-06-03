#
# MIT License
#
# Copyright (c) 2023 Jan Möller
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
CPMAddPackage("gh:jan-moeller/bugspray@0.2.0")

add_executable(${PROJECT_NAME}-tests
        test_assignability_from_related_error.cpp
        test_common_type.cpp
        test_constructibility_from_related_error.cpp
        test_default_constructibility.cpp
        test_transform.cpp
        test_type_identity.cpp
        test_use_with_expected.cpp
        test_value_constructibility.cpp
        test_visit.cpp
)
target_link_libraries(${PROJECT_NAME}-tests PUBLIC bugspray-with-main ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}-tests PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
target_compile_options(
        ${PROJECT_NAME}-tests
        PUBLIC
        "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->" # Turn off permissive mode on MSVC
        "$<$<COMPILE_LANG_AND_ID:CXX,GCC>:-Wall -Wextra -pedantic -Werror>"
)

add_test(NAME ${PROJECT_NAME}-tests COMMAND ${PROJECT_NAME}-tests)
