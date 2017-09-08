/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    tap.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FUNCTION_TAP_H
#define BOOST_FIT_GUARD_FUNCTION_TAP_H

/// tap
/// ===
/// 
/// Description
/// -----------
/// 
/// The `tap` function invokes a function on the first argument passed in and
/// then returns the first argument. This is useful in a chain of pipable
/// function to perform operations on intermediate results. As a result, this
/// function is [`pipable`](/include/boost/fit/pipable).
/// 
/// Synopsis
/// --------
/// 
///     template<class T, class F>
///     pipable constexpr T tap(T&& x, const F& f);
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [UnaryCallable](UnaryCallable)
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
///     #include <iostream>
///     using namespace boost::fit;
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
///     const pipable_adaptor<sum_f> sum = {};
///     int main() {
///         // Prints 3
///         int r = 1 | sum(2) | tap([](int i) { std::cout << i; }) | sum(2);
///         assert(r == 5);
///     }
/// 

#include <boost/fit/pipable.hpp>
#include <boost/fit/apply.hpp>
#include <boost/fit/detail/static_const_var.hpp>

namespace boost { namespace fit { namespace detail {

struct tap_f
{
    template<class T, class F>
    constexpr T operator()(T&& x, const F& f) const
    BOOST_FIT_RETURNS_DEDUCE_NOEXCEPT((boost::fit::apply(f, x), BOOST_FIT_FORWARD(T)(x)))
    {
        return boost::fit::apply(f, x), BOOST_FIT_FORWARD(T)(x);
    }
};

}

BOOST_FIT_DECLARE_STATIC_VAR(tap, pipable_adaptor<detail::tap_f>);


}} // namespace boost::fit

#endif
