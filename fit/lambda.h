/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    lambda.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_LAMBDA_H
#define FIT_GUARD_FUNCTION_LAMBDA_H

/// FIT_STATIC_LAMBDA
/// =================
/// 
/// Description
/// -----------
/// 
/// The `FIT_STATIC_LAMBDA` macro allows initializing lambdas at compile-time
/// in a `constexpr` expression.
/// 
/// Example
/// -------
/// 
///     const constexpr auto add_one = FIT_STATIC_LAMBDA(int x)
///     {
///         return x + 1;
///     };
/// 

#include <fit/function.h>

#define FIT_STATIC_LAMBDA FIT_DETAIL_MAKE_STATIC = []


#endif