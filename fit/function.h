/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    function.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_FUNCTION_H
#define FIT_GUARD_FUNCTION_FUNCTION_H

/// FIT_STATIC_FUNCTION
/// ===================
/// 
/// Description
/// -----------
/// 
/// The `FIT_STATIC_FUNCTION` macro allows initializing a function object from a
/// `constexpr` expression. It also ensures that the function object will have a
/// unique address across translation units. This helps to avoid ODR violations.
/// 
/// Example
/// -------
/// 
///     FIT_STATIC_FUNCTION(sum) = fit::partial([](int x, int y)
///     {
///         return x + y;
///     });
/// 

#include <fit/reveal.h>
#include <fit/detail/static_constexpr.h>
#include <fit/detail/static_const_var.h>

#if FIT_ONLY_DEFAULT_CONSTRUCTIBLE_STATIC_FUNCTION
#include <fit/lambda.h>
#endif

namespace fit {

namespace detail {

struct reveal_static_const_factory
{
#if FIT_NO_UNIQUE_STATIC_VAR
    template<class F>
    constexpr reveal_adaptor<F> operator=(const F&) const
    {
        return {};
    }
#else
    template<class F>
    constexpr const reveal_adaptor<F>& operator=(const F&) const
    {
        return static_const_var<reveal_adaptor<F>>();
    }
#endif
};
#if FIT_ONLY_DEFAULT_CONSTRUCTIBLE_STATIC_FUNCTION
struct reveal_function_factory
{
    template<class F>
    constexpr reveal_adaptor<F> operator += (F*)
    {
        return {};
    }
};
#endif

}}

#if FIT_ONLY_DEFAULT_CONSTRUCTIBLE_STATIC_FUNCTION
#define FIT_STATIC_FUNCTION(name) FIT_STATIC_CONSTEXPR auto name = \
    fit::detail::reveal_function_factory() += true ? nullptr : fit::detail::static_addr()
#elif FIT_NO_UNIQUE_STATIC_VAR
#define FIT_STATIC_FUNCTION(name) FIT_STATIC_CONSTEXPR auto name = fit::detail::reveal_static_const_factory()
#else
#define FIT_STATIC_FUNCTION(name) FIT_STATIC_AUTO_REF name = fit::detail::reveal_static_const_factory()
#endif


#endif
