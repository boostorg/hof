/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    in.cpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "example.h"

using namespace fit;

// Function to find an iterator using a containers built-in find if available
FIT_STATIC_LAMBDA_FUNCTION(find_iterator) = conditional(
    [](const std::string& s, const auto& x)
    {
        auto index = s.find(x);
        if (index == std::string::npos) return s.end();
        else return s.begin() + index;
    },
    [](const auto& r, const auto& x) -> decltype(r.find(x))
    {
        return r.find(x);
    },
    [](const auto& r, const auto& x)
    {
        using std::begin;
        using std::end;
        return std::find(begin(r), end(r), x);
    }
);
// Implement an infix `in` operator to check if a range contains an element
FIT_STATIC_LAMBDA_FUNCTION(in) = infix(
    [](const auto& x, const auto& r)
    {
        using std::end;
        return find_iterator(r, x) != end(r);
    }
);
// Negate version of `in`
FIT_STATIC_LAMBDA_FUNCTION(not_in) = infix(compose(not _, in));

int main()
{
    // Check if vector contains element
    std::vector<int> numbers = { 1, 2, 3, 4, 5 };
    if (5 <in> numbers) std::cout << "Yes" << std::endl;

    // Check if string contains element
    std::string s = "hello world";
    if ("hello" <in> s) std::cout << "Yes" << std::endl;

    // Check if map contains element
    std::map<int, std::string> number_map = {
        { 1, "1" },
        { 2, "2" },
        { 3, "3" },
        { 4, "4" }
    };

    if (4 <in> number_map) std::cout << "Yes" << std::endl;

    // Check if map doesn't contains element
    if (not (8 <in> numbers)) std::cout << "No" << std::endl;
    if (8 <not_in> numbers) std::cout << "No" << std::endl;

}

