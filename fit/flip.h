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

#include <fit/detail/result_of.h>
#include <fit/reveal.h>
#include <fit/detail/make.h>
#include <fit/detail/static_const_var.h>

namespace fit {

template<class F>
struct flip_adaptor : F
{
    typedef flip_adaptor fit_rewritable1_tag;
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
    constexpr FIT_SFINAE_RESULT(const F&, id_<U>, id_<T>, id_<Ts>...) 
    operator()(T&& x, U&& y, Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        (FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...)))
            (fit::forward<U>(y), fit::forward<T>(x), fit::forward<Ts>(xs)...)
    );
};

FIT_DECLARE_STATIC_VAR(flip, detail::make<flip_adaptor>);

}

#endif