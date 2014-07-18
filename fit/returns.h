/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    returns.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_RETURNS_H
#define FIT_GUARD_RETURNS_H

#include <utility>

#define FIT_RETURNS(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; } static_assert(true, "")

#define FIT_AUTO_FORWARD(x) std::forward<decltype(x)>(x)

#endif