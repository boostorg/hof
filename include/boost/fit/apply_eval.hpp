/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    apply_eval.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_APPLY_EVAL_H
#define BOOST_FIT_GUARD_APPLY_EVAL_H

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
/// * [ConstCallable](concepts.md#constcallable)
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
///     assert(boost::fit::apply_eval(sum_f(), []{ return 1; }, []{ return 2; }) == 3);
/// 

#include <boost/fit/returns.hpp>
#include <boost/fit/detail/forward.hpp>
#include <boost/fit/detail/static_const_var.hpp>
#include <boost/fit/apply.hpp>
#include <boost/fit/eval.hpp>

#ifndef BOOST_FIT_NO_ORDERED_BRACE_INIT
#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 9) || defined(_MSC_VER)
#define BOOST_FIT_NO_ORDERED_BRACE_INIT 1
#else
#define BOOST_FIT_NO_ORDERED_BRACE_INIT 0
#endif 
#endif

#if BOOST_FIT_NO_ORDERED_BRACE_INIT
#include <boost/fit/pack.hpp>
#include <boost/fit/capture.hpp>
#endif

namespace boost { namespace fit {

namespace detail {

#if BOOST_FIT_NO_ORDERED_BRACE_INIT
template<class R, class F, class Pack>
constexpr R eval_ordered(const F& f, Pack&& p)
{
    return p(f);
}

template<class R, class F, class Pack, class T, class... Ts>
constexpr R eval_ordered(const F& f, Pack&& p, T&& x, Ts&&... xs)
{
    return eval_ordered<R>(f, pack_join(BOOST_FIT_FORWARD(Pack)(p), boost::fit::pack_forward(boost::fit::eval(x))), BOOST_FIT_FORWARD(Ts)(xs)...);
}
#else
template<class R>
struct eval_helper
{
    R result;

    template<class F, class... Ts>
    constexpr eval_helper(const F& f, Ts&&... xs) : result(apply(f, BOOST_FIT_FORWARD(Ts)(xs)...))
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
    constexpr eval_helper(const F& f, Ts&&... xs) : x(apply(f, BOOST_FIT_FORWARD(Ts)(xs)...), 0)
    {}
};
#endif

struct apply_eval_f
{
    template<class F, class... Ts, class R=decltype(
        apply(std::declval<const F&>(), boost::fit::eval(std::declval<Ts>())...)
    ),
    class=typename std::enable_if<(!std::is_void<R>::value)>::type 
    >
    constexpr R operator()(const F& f, Ts&&... xs) const
    {
        return
#if BOOST_FIT_NO_ORDERED_BRACE_INIT
        eval_ordered<R>
            (f, pack(), BOOST_FIT_FORWARD(Ts)(xs)...);
#else
        eval_helper<R>
            {f, boost::fit::eval(BOOST_FIT_FORWARD(Ts)(xs))...}.get_result();
#endif
    }

    template<class F, class... Ts, class R=decltype(
        apply(std::declval<const F&>(), boost::fit::eval(std::declval<Ts>())...)
    ),
    class=typename std::enable_if<(std::is_void<R>::value)>::type 
    >
    constexpr typename detail::holder<Ts...>::type 
    operator()(const F& f, Ts&&... xs) const
    {
        return (typename detail::holder<Ts...>::type)
#if BOOST_FIT_NO_ORDERED_BRACE_INIT
        eval_ordered<R>
            (f, pack(), BOOST_FIT_FORWARD(Ts)(xs)...);
#else
        eval_helper<R>
            {f, boost::fit::eval(BOOST_FIT_FORWARD(Ts)(xs))...};
#endif
    }
};

}

BOOST_FIT_DECLARE_STATIC_VAR(apply_eval, detail::apply_eval_f);

}} // namespace boost::fit

#endif
