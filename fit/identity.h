/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    identity.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_IDENTITY_H
#define FIT_GUARD_FUNCTION_IDENTITY_H

/// identity
/// ========
/// 
/// Description
/// -----------
/// 
/// The `identity` function is an unary function object that returns whats given to it. 
/// 
/// Synopsis
/// --------
/// 
///     struct
///     {
///         template<class T>
///         constexpr T operator()(T&& x) const
///         {
///             return fit::forward<T>(x);
///         }
///     } identity;
/// 

#include <utility>

namespace fit { namespace detail {

struct identity_base
{
    template<class T>
    constexpr T operator()(T&& x) const
    {
        return std::forward<T>(x);
    }
};

}

constexpr detail::identity_base identity = {};

}

#endif
