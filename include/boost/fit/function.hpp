/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    function.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FUNCTION_FUNCTION_H
#define BOOST_FIT_GUARD_FUNCTION_FUNCTION_H

/// BOOST_FIT_STATIC_FUNCTION
/// ===================
/// 
/// Description
/// -----------
/// 
/// The `BOOST_FIT_STATIC_FUNCTION` macro allows initializing a function object from
/// a `constexpr` expression. It also ensures that the function object will
/// have a unique address across translation units. This helps to avoid ODR
/// violations. As such, the object that is deduced is default constructed.
/// 
/// By default, all functions defined with `BOOST_FIT_STATIC_FUNCTION` use the
/// `boost::fit::reveal` adaptor to improve error messages.
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
/// 
///     BOOST_FIT_STATIC_FUNCTION(sum) = sum_f();
///     BOOST_FIT_STATIC_FUNCTION(partial_sum) = boost::fit::partial(sum_f());
///     assert(sum(1, 2) == partial_sum(1)(2));
/// 

#include <boost/fit/reveal.hpp>
#include <boost/fit/detail/static_constexpr.hpp>
#include <boost/fit/detail/static_const_var.hpp>
#include <boost/fit/detail/constexpr_deduce.hpp>

namespace boost { namespace fit {

namespace detail {

struct reveal_static_const_factory
{
    constexpr reveal_static_const_factory()
    {}
#if BOOST_FIT_NO_UNIQUE_STATIC_VAR
    template<class F>
    constexpr reveal_adaptor<F> operator=(const F& f) const
    {
        static_assert(BOOST_FIT_IS_DEFAULT_CONSTRUCTIBLE(F), "Static functions must be default constructible");
        return reveal_adaptor<F>(f);
    }
#else
    template<class F>
    constexpr const reveal_adaptor<F>& operator=(const F&) const
    {
        static_assert(BOOST_FIT_IS_DEFAULT_CONSTRUCTIBLE(F), "Static functions must be default constructible");
        return static_const_var<reveal_adaptor<F>>();
    }
#endif
};
}}} // namespace boost::fit

#if BOOST_FIT_NO_UNIQUE_STATIC_VAR
#define BOOST_FIT_STATIC_FUNCTION(name) BOOST_FIT_STATIC_CONSTEXPR auto name = BOOST_FIT_DETAIL_MSVC_CONSTEXPR_DEDUCE boost::fit::detail::reveal_static_const_factory()
#else
#define BOOST_FIT_STATIC_FUNCTION(name) BOOST_FIT_STATIC_AUTO_REF name = boost::fit::detail::reveal_static_const_factory()
#endif


#endif
