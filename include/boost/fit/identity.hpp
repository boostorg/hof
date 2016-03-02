/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    identity.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FUNCTION_IDENTITY_H
#define BOOST_FIT_GUARD_FUNCTION_IDENTITY_H

/// identity
/// ========
/// 
/// Description
/// -----------
/// 
/// The `identity` function is an unary function object that returns whats given to it. 
/// 
/// Semantics
/// ---------
/// 
///     assert(identity(x) == x);
/// 
/// Synopsis
/// --------
/// 
///     struct
///     {
///         template<class T>
///         constexpr T operator()(T&& x) const
///         {
///             return boost::fit::forward<T>(x);
///         }
///     } identity;
/// 

#include <utility>
#include <boost/fit/detail/forward.hpp>
#include <boost/fit/detail/static_const_var.hpp>

namespace boost { namespace fit { namespace detail {

struct identity_base
{
    template<class T>
    constexpr T operator()(T&& x) const
    {
        return BOOST_FIT_FORWARD(T)(x);
    }
};

}

BOOST_FIT_DECLARE_STATIC_VAR(identity, detail::identity_base);

}} // namespace boost::fit

#endif
