/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    is_callable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_HOF_GUARD_IS_CALLABLE_H
#define BOOST_HOF_GUARD_IS_CALLABLE_H

/// is_callable
/// ===========
/// 
/// Description
/// -----------
/// 
/// The `is_callable` metafunction checks if the function is callable with
/// certain parameters.
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [Callable](Callable)
/// 
/// Synopsis
/// --------
/// 
///     template<class F, class... Ts>
///     struct is_callable;
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     using namespace boost::hof;
/// 
///     struct is_callable_class
///     {
///         void operator()(int) const
///         {
///         }
///     };
///     static_assert(is_callable<is_callable_class, int>(), "Not callable");
/// 
///     int main() {}
/// 


#include <boost/hof/detail/can_be_called.hpp>
#include <boost/hof/apply.hpp>

namespace boost { namespace hof {

template<class F, class... Ts>
struct is_callable 
: detail::can_be_called<detail::apply_f, F, Ts...>
{};

template<class F, class... Ts, class... Us>
struct is_callable<F(Ts...), Us...>
{
    static_assert(!std::is_same<F, F>::value, 
        "The is_callable<F(Args...)> form is not supported because it is problematic."
        "Please use is_callable<F, Args...> instead."
    );
};

}} // namespace boost::hof

#endif
