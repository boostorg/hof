/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    each_arg.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_EACH_ARG_H
#define FIT_GUARD_EACH_ARG_H

#include <fit/detail/forward.h>
#include <fit/detail/static_constexpr.h>
#include <initializer_list>

/// each_arg
/// ========
/// 
/// Description
/// -----------
/// 
/// The `each_arg` function will apply the function to each argument that is
/// passed in.
/// 
/// Synopsis
/// --------
/// 
///     template <class F, class... Ts> 
///     constexpr void each_arg(F&& f, Ts&&... a);
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
///     FunctionObject
/// 
/// Example
/// -------
/// 
///     std::string s;
///     auto f = [&](const std::string& x)
///     {
///         s += x;
///     };
///     fit::each_arg(f, "hello", "-", "world");
///     assert(s == "hello-world");
/// 

namespace fit {

namespace detail {

struct each_arg_f
{
    template <class F, class... Ts> 
    constexpr void operator()(F&& f, Ts&&... a) const
    { 
        (void)std::initializer_list<int>{(f(fit::forward<Ts>(a)), 0)...}; 
    }
};
}

FIT_STATIC_CONSTEXPR detail::each_arg_f each_arg = {};

}

#endif