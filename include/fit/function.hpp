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
/// The `FIT_STATIC_FUNCTION` macro allows initializing a function object from
/// a `constexpr` expression. It also ensures that the function object will
/// have a unique address across translation units. This helps to avoid ODR
/// violations. As such, the object that is deduced is default constructed.
/// 
/// By default, all functions defined with `FIT_STATIC_FUNCTION` use the
/// [`fit::reveal`](/include/fit/reveal) adaptor to improve error messages.
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
///     FIT_STATIC_FUNCTION(sum) = sum_f();
///     FIT_STATIC_FUNCTION(partial_sum) = fit::partial(sum_f());
/// 
///     int main() {
///         assert(sum(1, 2) == partial_sum(1)(2));
///     }
/// 

#include <fit/reveal.hpp>
#include <fit/detail/static_const_var.hpp>
#include <fit/detail/constexpr_deduce.hpp>

namespace fit {

namespace detail {

struct reveal_static_const_factory
{
    constexpr reveal_static_const_factory()
    {}
#if FIT_NO_UNIQUE_STATIC_VAR
    template<class F>
    constexpr reveal_adaptor<F> operator=(const F& f) const
    {
        static_assert(FIT_IS_DEFAULT_CONSTRUCTIBLE(F), "Static functions must be default constructible");
        return reveal_adaptor<F>(f);
    }
#else
    template<class F>
    constexpr const reveal_adaptor<F>& operator=(const F&) const
    {
        static_assert(FIT_IS_DEFAULT_CONSTRUCTIBLE(F), "Static functions must be default constructible");
        return fit::static_const_var<reveal_adaptor<F>>();
    }
#endif
};
}} // namespace fit

#if FIT_NO_UNIQUE_STATIC_VAR
#define FIT_STATIC_FUNCTION(name) FIT_STATIC_CONSTEXPR auto name = FIT_DETAIL_MSVC_CONSTEXPR_DEDUCE fit::detail::reveal_static_const_factory()
#else
#define FIT_STATIC_FUNCTION(name) FIT_STATIC_AUTO_REF name = fit::detail::reveal_static_const_factory()
#endif


#endif
