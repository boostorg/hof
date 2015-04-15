/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    flip.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FLIP_H
#define FIT_GUARD_FLIP_H

/// flip
/// ====
/// 
/// Description
/// -----------
/// 
/// The `flip` function adaptor swaps the first two parameters.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     flip_adaptor<F> flip(F f);
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
///     BinaryFunctionObject
///     MoveConstructible
/// 
/// Example
/// -------
/// 
///     int r = fit::flip(fit::_ - fit::_)(2, 5);
///     assert(r == 3);
/// 

#include <fit/returns.h>
#include <fit/reveal.h>
#include <fit/detail/make.h>
#include <fit/detail/static_constexpr.h>

namespace fit {

template<class F>
struct flip_adaptor : F
{
    FIT_INHERIT_CONSTRUCTOR(flip_adaptor, F);

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    struct flip_failure
    {
        template<class Failure>
        struct apply
        {
            template<class T, class U, class... Ts>
            struct of
            : Failure::template of<U, T, Ts...>
            {};
        };
    };

    struct failure
    : failure_map<flip_failure, F>
    {};

    FIT_RETURNS_CLASS(flip_adaptor);

    template<class T, class U, class... Ts>
    constexpr auto operator()(T&& x, U&& y, Ts&&... xs) const FIT_RETURNS
    (
        (FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...)))
            (fit::forward<U>(y), fit::forward<T>(x), fit::forward<Ts>(xs)...)
    );
};

FIT_STATIC_CONSTEXPR detail::make<flip_adaptor> flip = {};

}

#endif