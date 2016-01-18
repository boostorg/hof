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
/// The `identity` function is a unary function object that returns whats given to it. 
/// 
/// Semantics
/// ---------
/// 
///     assert(idenity(x) == x);
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
#include <fit/detail/forward.hpp>
#include <fit/detail/static_const_var.hpp>

namespace fit { namespace detail {

struct identity_base
{
    template<class T>
    constexpr T operator()(T&& x) const
    {
        return fit::forward<T>(x);
    }
};

}

FIT_DECLARE_STATIC_VAR(identity, detail::identity_base);

} // namespace fit

#endif
