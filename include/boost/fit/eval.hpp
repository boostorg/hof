/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    eval.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_EVAL_H
#define BOOST_FIT_GUARD_EVAL_H

/// eval
/// ====
/// 
/// Description
/// -----------
/// 
/// The `eval` function will evaluate a "thunk". This can be either a nullary
/// function or it can be a unary function that takes the identity function as
/// the first parameter(which is helpful to delay compile-time checking).
/// Also, additional parameters can be passed to `eval` to delay
/// compiliation(so that result can depend on template parameters).
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
/// * [EvaluatableFunctionObject](concepts.md#evaluatablefunctionobject)
/// 
/// Example
/// -------
/// 
///     assert(boost::fit::eval([]{ return 3; }) == 3);
/// 

#include <boost/fit/always.hpp>
#include <boost/fit/identity.hpp>
#include <boost/fit/conditional.hpp>
#include <boost/fit/detail/result_of.hpp>

namespace boost { namespace fit {

namespace detail {

struct simple_eval
{
    template<class F, class... Ts>
    constexpr BOOST_FIT_SFINAE_RESULT(F) 
    operator()(F&& f, Ts&&...xs) const BOOST_FIT_SFINAE_RETURNS
    (boost::fit::always_ref(f)(xs...)());
};

struct id_eval
{
    template<class F, class... Ts>
    constexpr BOOST_FIT_SFINAE_RESULT(F, id_<decltype(boost::fit::identity)>) 
    operator()(F&& f, Ts&&...xs) const BOOST_FIT_SFINAE_RETURNS
    (boost::fit::always_ref(f)(xs...)(boost::fit::identity));
};

}

BOOST_FIT_DECLARE_STATIC_VAR(eval, boost::fit::conditional_adaptor<detail::simple_eval, detail::id_eval>);

}} // namespace boost::fit

#endif
