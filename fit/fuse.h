/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    fuse.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_FUSE_H
#define FIT_GUARD_FUNCTION_FUSE_H

/// fuse
/// ====
/// 
/// Description
/// -----------
/// 
/// The `fuse` function adaptor takes a tuple and uses that for the arguments
/// to the function.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     fuse_adaptor<F> fuse(F f);
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
///     FunctionObject
///     MoveConstructible
/// 
/// Example
/// -------
/// 
///     struct sum
///     {
///         template<class T, class U>
///         T operator()(T x, U y) const
///         {
///             return x+y;
///         }
///     };
/// 
///     int r = fuse(sum())(std::make_tuple(3,2));
///     assert(r == 5);
/// 

#include <fit/invoke.h>
#include <fit/variadic.h>
#include <fit/always.h>
#include <fit/detail/delegate.h>
#include <fit/detail/move.h>
#include <fit/detail/make.h>
#include <fit/detail/static_constexpr.h>

namespace fit {

template<class F>
struct fuse_adaptor : F
{
    FIT_INHERIT_CONSTRUCTOR(fuse_adaptor, F);

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(fuse_adaptor);

    template<class T>
    constexpr auto operator()(T && x) const
    FIT_RETURNS
    (
        fit::invoke(FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(x)), fit::forward<T>(x))
    );
};

// Optimizations
template<class F>
struct fuse_adaptor<variadic_adaptor<F> > : F
{
    FIT_INHERIT_CONSTRUCTOR(fuse_adaptor, F);
};

template<class F>
struct variadic_adaptor<fuse_adaptor<F> > : F
{
    FIT_INHERIT_CONSTRUCTOR(variadic_adaptor, F);
};

FIT_STATIC_CONSTEXPR detail::make<fuse_adaptor> fuse = {};

}

#endif
