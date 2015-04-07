/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    always.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_ALWAYS_H
#define FIT_GUARD_FUNCTION_ALWAYS_H

#include <fit/detail/unwrap.h>
#include <fit/detail/static_constexpr.h>

/// always
/// ======
/// 
/// Description
/// -----------
/// 
/// The `always` function returns a function object that will always return
/// the value given to it, no matter what parameters are passed to the
/// function object. The `always_ref` version will return a reference, and it
/// requires the value passed in to be an lvalue.
/// 
/// Synopsis
/// --------
/// 
///     template<class T>
///     constexpr /* unspecified */ always(T value);
/// 
///     template<class T>
///     constexpr /* unspecified */ always_ref(T& value);
/// 
/// Requirements
/// ------------
/// 
/// T must be:
/// 
///     CopyConstructible
/// 
/// Example
/// -------
/// 
///     int ten = 10;
///     assert( always(ten)(1,2,3,4,5) == 10 );
/// 

namespace fit { namespace detail {

template<class T>
struct always_base
{
    T x;
    
    constexpr always_base()
    {}
    
    constexpr always_base(T x) : x(x)
    {}

    template<class... As>
    constexpr typename unwrap_reference<T>::type 
    operator()(As&&...) const
    {
        return this->x;
    }
};

struct always_f
{
    template<class T>
    constexpr detail::always_base<T> operator()(T x) const
    {
        return detail::always_base<T>(x);
    }
};

struct always_ref_f
{
    template<class T>
    constexpr detail::always_base<T&> operator()(T& x) const
    {
        return detail::always_base<T&>(x);
    }
};

}

FIT_STATIC_CONSTEXPR detail::always_f always = {};
FIT_STATIC_CONSTEXPR detail::always_ref_f always_ref = {};

}

#endif
