/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    protect.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_PROTECT_H
#define FIT_GUARD_FUNCTION_PROTECT_H

/// protect
/// =======
/// 
/// Description
/// -----------
/// 
/// The `protect` function adaptor can be used to make a bind expression be
/// treated a regular function instead.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr protect_adaptor<F> protect(F f);
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
///     FunctionObject
///     MoveConstructible
/// 

#include <utility>
#include <fit/reveal.h>
#include <fit/detail/forward.h>
#include <fit/detail/make.h>
#include <fit/detail/static_constexpr.h>

namespace fit {

template<class F>
struct protect_adaptor : F
{
    template<class... Ts>
    constexpr protect_adaptor(Ts&&... xs) : F(fit::forward<Ts>(xs)...)
    {}
};

FIT_STATIC_CONSTEXPR detail::make<protect_adaptor> protect = {};

}
#endif
