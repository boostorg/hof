/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    apply.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_APPLY_H
#define FIT_GUARD_APPLY_H

/// apply
/// =====
/// 
/// Description
/// -----------
/// 
/// The `apply` function calls the function given to it with its arguments.
/// 
/// Synopsis
/// --------
/// 
///     template<class F, class... Ts>
///     constexpr auto apply(F&& f, Ts&&... xs);
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
///     struct sum_f
///     {
///         template<class T, class U>
///         T operator()(T x, U y) const
///         {
///             return x+y;
///         }
///     };
///     assert(fit::apply(sum_f(), 1, 2) == 3);
/// 

#include <fit/returns.h>
#include <fit/detail/forward.h>
#include <fit/detail/static_constexpr.h>

namespace fit {

namespace detail {

struct apply_f
{
    template<class F, class... Ts>
    constexpr auto operator()(F&& f, Ts&&... xs) const FIT_RETURNS
    (
        f(fit::forward<Ts>(xs)...)
    );
};

}

FIT_STATIC_CONSTEXPR detail::apply_f apply = {};

}

#endif
