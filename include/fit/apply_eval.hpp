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
/// The `apply_eval` function work like [`apply`](/include/fit/apply), except it calls
/// [`eval`](/include/fit/eval) on each of its arguments. Each [`eval`](/include/fit/eval) call is
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
/// * [ConstCallable](ConstCallable)
/// 
/// Ts must be:
/// 
/// * [EvaluatableFunctionObject](EvaluatableFunctionObject)
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
/// 
///     struct sum_f
///     {
///         template<class T, class U>
///         T operator()(T x, U y) const
///         {
///             return x+y;
///         }
///     };
/// 
///     int main() {
///         assert(fit::apply_eval(sum_f(), []{ return 1; }, []{ return 2; }) == 3);
///     }
/// 

#include <fit/config.hpp>
#include <fit/returns.hpp>
#include <fit/detail/forward.hpp>
#include <fit/detail/static_const_var.hpp>
#include <fit/apply.hpp>
#include <fit/eval.hpp>

#if FIT_NO_ORDERED_BRACE_INIT
#include <fit/pack.hpp>
#include <fit/capture.hpp>
#endif

namespace fit {

namespace detail {

#if FIT_NO_ORDERED_BRACE_INIT
template<class R, class F, class Pack>
constexpr R eval_ordered(const F& f, Pack&& p)
{
    return p(f);
}

template<class R, class F, class Pack, class T, class... Ts>
constexpr R eval_ordered(const F& f, Pack&& p, T&& x, Ts&&... xs)
{
    return fit::detail::eval_ordered<R>(f, fit::pack_join(FIT_FORWARD(Pack)(p), fit::pack_forward(fit::eval(x))), FIT_FORWARD(Ts)(xs)...);
}
#else
template<class R>
struct eval_helper
{
    R result;

    template<class F, class... Ts>
    constexpr eval_helper(const F& f, Ts&&... xs) : result(fit::apply(f, FIT_FORWARD(Ts)(xs)...))
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
    constexpr eval_helper(const F& f, Ts&&... xs) : x((fit::apply(f, FIT_FORWARD(Ts)(xs)...), 0))
    {}
};
#endif

struct apply_eval_f
{
    template<class F, class... Ts, class R=decltype(
        fit::apply(std::declval<const F&>(), fit::eval(std::declval<Ts>())...)
    ),
    class=typename std::enable_if<(!std::is_void<R>::value)>::type 
    >
    constexpr R operator()(const F& f, Ts&&... xs) const FIT_RETURNS_DEDUCE_NOEXCEPT(fit::apply(f, fit::eval(FIT_FORWARD(Ts)(xs))...))
    {
        return
#if FIT_NO_ORDERED_BRACE_INIT
        detail::eval_ordered<R>
            (f, fit::pack(), FIT_FORWARD(Ts)(xs)...);
#else
        detail::eval_helper<R>
            {f, fit::eval(FIT_FORWARD(Ts)(xs))...}.get_result();
#endif
    }

    template<class F, class... Ts, class R=decltype(
        fit::apply(std::declval<const F&>(), fit::eval(std::declval<Ts>())...)
    ),
    class=typename std::enable_if<(std::is_void<R>::value)>::type 
    >
    constexpr typename detail::holder<Ts...>::type 
    operator()(const F& f, Ts&&... xs) const FIT_RETURNS_DEDUCE_NOEXCEPT(fit::apply(f, fit::eval(FIT_FORWARD(Ts)(xs))...))
    {
        return (typename detail::holder<Ts...>::type)
#if FIT_NO_ORDERED_BRACE_INIT
        detail::eval_ordered<R>
            (f, fit::pack(), FIT_FORWARD(Ts)(xs)...);
#else
        detail::eval_helper<R>
            {f, fit::eval(FIT_FORWARD(Ts)(xs))...};
#endif
    }
};

}

FIT_DECLARE_STATIC_VAR(apply_eval, detail::apply_eval_f);

} // namespace fit

#endif
