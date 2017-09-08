/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    pointfree.cpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "example.h"

using namespace fit;

FIT_STATIC_FUNCTION(simple_print) = FIT_LIFT(std::ref(std::cout) << _);
FIT_STATIC_FUNCTION(print) = by(simple_print);
FIT_STATIC_FUNCTION(print_lines) = by(flow(simple_print, _ << std::integral_constant<char, '\n'>{}));
FIT_STATIC_FUNCTION(max) = fold(FIT_LIFT(std::max));

int main() 
{
    simple_print("Hello\n");
    print("Hello", "World\n");
    print_lines("Hello", "World");

    auto n = max(1, 2, 4, 3); // Returns 4
    auto m = max(0.1, 0.2, 0.5, 0.4); // Returns 0.5

    print_lines(n, m);
}
