/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    apply_eval.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_APPLY_EVAL_H
#define FIT_GUARD_APPLY_EVAL_H

/// apply_eval
/// ==========
/// 
/// Description
/// -----------
/// 
/// The `apply_eval` function work like apply, except it calls `eval` on each of
/// its arguments. Each `eval` call is always ordered from left-to-right.
/// 
/// Synopsis
/// --------
/// 
///     template<class F, class... Ts>
///     constexpr auto apply_eval(F&& f, Ts&&... xs);
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
///     FunctionObject
/// 
/// Ts must be:
/// 
///     Evaluatable
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
///     assert(fit::apply_eval(sum_f(), []{ return 1; }, []{ return 2; }) == 3);
/// 

#include <fit/returns.h>
#include <fit/detail/forward.h>
#include <fit/detail/static_constexpr.h>
#include <fit/eval.h>

#ifndef FIT_NO_ORDERD_BRACE_INIT
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 9
#define FIT_NO_ORDERD_BRACE_INIT 1
#else
#define FIT_NO_ORDERD_BRACE_INIT 0
#endif 
#endif

#if FIT_NO_ORDERD_BRACE_INIT
#include <fit/pack.h>
#include <fit/capture.h>
#endif

namespace fit {

namespace detail {

#if FIT_NO_ORDERD_BRACE_INIT
template<class R, class F, class Pack>
constexpr R eval_ordered(const F& f, Pack&& p)
{
    return p(f);
}

template<class R, class F, class Pack, class T, class... Ts>
constexpr R eval_ordered(const F& f, Pack&& p, T&& x, Ts&&... xs)
{
    return eval_ordered<R>(f, pack_join(p, fit::pack(fit::eval(x))), fit::forward<Ts>(xs)...);
}
#else
template<class R>
struct eval_helper
{
    R result;

    template<class F, class... Ts>
    constexpr eval_helper(const F& f, Ts&&... xs) : result(f(fit::forward<Ts>(xs)...))
    {}

    constexpr R get_result()
    {
        return (R&&)result;
    }
};
#endif

struct apply_eval_f
{
    template<class F, class... Ts, class R=decltype(
        std::declval<const F&>()(fit::eval(std::declval<Ts>())...)
    )>
    constexpr R operator()(const F& f, Ts&&... xs) const
    {
        return
#if FIT_NO_ORDERD_BRACE_INIT
        eval_ordered<R>
            (f, pack(), fit::forward<Ts>(xs)...);
#else
        eval_helper<R>
            {f, fit::eval(fit::forward<Ts>(xs))...}.get_result();
#endif
    }
};

}

FIT_STATIC_CONSTEXPR detail::apply_eval_f apply_eval = {};

}

#endif
