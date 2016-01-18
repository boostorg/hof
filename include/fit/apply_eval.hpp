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
/// The `apply_eval` function work like [`apply`](apply.md), except it calls
/// [`eval`](eval.md) on each of its arguments. Each [`eval`](eval.md) call is
/// always ordered from left-to-right.
/// 
/// Synopsis
/// --------
/// 
///     template<class F, class... Ts>
///     constexpr auto apply_eval(F&& f, Ts&&... xs);
/// 
/// Semantics
/// ---------
/// 
///     assert(apply_eval(f)(xs...) == f(eval(xs)...));
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [Callable](concepts.md#callable)
/// 
/// Ts must be:
/// 
/// * [EvaluatableFunctionObject](concepts.md#evaluatablefunctionobject)
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

#include <fit/returns.hpp>
#include <fit/detail/forward.hpp>
#include <fit/detail/static_const_var.hpp>
#include <fit/apply.hpp>
#include <fit/eval.hpp>

#ifndef FIT_NO_ORDERD_BRACE_INIT
#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 9) || defined(_MSC_VER)
#define FIT_NO_ORDERD_BRACE_INIT 1
#else
#define FIT_NO_ORDERD_BRACE_INIT 0
#endif 
#endif

#if FIT_NO_ORDERD_BRACE_INIT
#include <fit/pack.hpp>
#include <fit/capture.hpp>
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
    return eval_ordered<R>(f, pack_join(fit::forward<Pack>(p), fit::pack_forward(fit::eval(x))), fit::forward<Ts>(xs)...);
}
#else
template<class R>
struct eval_helper
{
    R result;

    template<class F, class... Ts>
    constexpr eval_helper(const F& f, Ts&&... xs) : result(apply(f, fit::forward<Ts>(xs)...))
    {}

    constexpr R get_result()
    {
        return (R&&)result;
    }
};

template<>
struct eval_helper<void>
{
    int x;
    template<class F, class... Ts>
    constexpr eval_helper(const F& f, Ts&&... xs) : x(apply(f, fit::forward<Ts>(xs)...), 0)
    {}
};
#endif

struct apply_eval_f
{
    template<class F, class... Ts, class R=decltype(
        apply(std::declval<const F&>(), fit::eval(std::declval<Ts>())...)
    ),
    class=typename std::enable_if<(!std::is_void<R>::value)>::type 
    >
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

    template<class F, class... Ts, class R=decltype(
        apply(std::declval<const F&>(), fit::eval(std::declval<Ts>())...)
    ),
    class=typename std::enable_if<(std::is_void<R>::value)>::type 
    >
    constexpr void operator()(const F& f, Ts&&... xs) const
    {
#if FIT_NO_ORDERD_BRACE_INIT
        eval_ordered<R>
            (f, pack(), fit::forward<Ts>(xs)...);
#else
        eval_helper<R>
            {f, fit::eval(fit::forward<Ts>(xs))...};
#endif
    }
};

}

FIT_DECLARE_STATIC_VAR(apply_eval, detail::apply_eval_f);

} // namespace fit

#endif
