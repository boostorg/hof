/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    eval.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_EVAL_H
#define FIT_GUARD_EVAL_H

/// eval
/// ====
/// 
/// Description
/// -----------
/// 
/// The `eval` function will evaluate a "thunk". This can be either a nullary
/// function or it can be a an unary function that takes the identity function
/// as the first parameter(which is helpful to delay compile-time checking).
/// Also, additional parameters can be passed to `eval` to delay compiliation(so
/// that result can depend on template parameters).
/// 
/// Synopsis
/// --------
/// 
///     template<class F, class... Ts>
///     constexpr auto eval(F&& f, Ts&&...);
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
///     EvaluatableFunctionObject
/// 
/// Example
/// -------
/// 
///     assert(fit::eval([]{ return 3; }) == 3);
/// 

#include <fit/always.h>
#include <fit/identity.h>
#include <fit/conditional.h>
#include <fit/returns.h>

namespace fit {

namespace detail {

struct simple_eval
{
    template<class F, class... Ts>
    constexpr auto operator()(F&& f, Ts&&...xs) const FIT_RETURNS
    (fit::always_ref(f)(xs...)());
};

struct id_eval
{
    template<class F, class... Ts>
    constexpr auto operator()(F&& f, Ts&&...xs) const FIT_RETURNS
    (fit::always_ref(f)(xs...)(fit::identity));
};

}

FIT_DECLARE_STATIC_VAR(eval, fit::conditional_adaptor<detail::simple_eval, detail::id_eval>);

}

#endif
