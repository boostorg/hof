/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    enable_if_convertible.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_ENABLE_IF_CONVERTIBLE_H
#define FIT_GUARD_FUNCTION_ENABLE_IF_CONVERTIBLE_H

#include <utility>

#define FIT_ENABLE_IF_CONVERTIBLE(T, U) \
	typename std::enable_if<std::is_convertible<T, U>::value, int>::type = 0

#endif