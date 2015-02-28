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
/// lambdas and adaptors at compile-time in a `constexpr` expression.
/// 
/// Example
/// -------
/// 
///     FIT_STATIC_FUNCTION(sum) = fit::partial([](int x, int y)
///     {
///         return x + y;
///     });
/// 

#include <type_traits>
#include <utility>
#include <fit/returns.h>
#include <fit/reveal.h>
#include <fit/detail/static_constexpr.h>

namespace fit {

namespace detail {
template<class F>
struct static_function_wrapper
{
    static_assert(std::is_empty<F>::value, "Function or lambda expression must be empty");

    struct failure
    : failure_for<F>
    {};

    template<class... Ts>
    const F& base_function(Ts&&...) const
    {
        return reinterpret_cast<const F&>(*this);
    }

    FIT_RETURNS_CLASS(static_function_wrapper);

    template<class... Ts>
    auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        FIT_RETURNS_REINTERPRET_CAST(const F&)(*FIT_CONST_THIS)(fit::forward<Ts>(xs)...)
    );
};

struct static_function_wrapper_factor
{
    template<class F>
    constexpr static_function_wrapper<F> operator += (F*)
    {
        static_assert(std::is_literal_type<static_function_wrapper<F>>::value, "Function wrapper not a literal type");
        return {};
    }
};

struct reveal_static_function_wrapper_factor
{
    template<class F>
    constexpr reveal_adaptor<static_function_wrapper<F>> operator += (F*)
    {
        return {};
    }
};

struct static_addr
{
    template<class T>
    typename std::remove_reference<T>::type *operator=(T &&t) const
    {
        return &t;
    }
};

}}

#define FIT_DETAIL_MAKE_STATIC fit::detail::static_function_wrapper_factor() += true ? nullptr : fit::detail::static_addr()
#define FIT_DETAIL_MAKE_REVEAL_STATIC fit::detail::reveal_static_function_wrapper_factor() += true ? nullptr : fit::detail::static_addr()
#define FIT_STATIC_FUNCTION(name) FIT_STATIC_CONSTEXPR auto name = FIT_DETAIL_MAKE_REVEAL_STATIC


#endif
