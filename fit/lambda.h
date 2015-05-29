/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    lambda.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_LAMBDA_H
#define FIT_GUARD_FUNCTION_LAMBDA_H

/// FIT_STATIC_LAMBDA
/// =================
/// 
/// Description
/// -----------
/// 
/// The `FIT_STATIC_LAMBDA` macro allows initializing non-capturing lambdas at
/// compile-time in a `constexpr` expression.
/// 
/// Example
/// -------
/// 
///     const constexpr auto add_one = FIT_STATIC_LAMBDA(int x)
///     {
///         return x + 1;
///     };
/// 
/// FIT_STATIC_LAMBDA_FUNCTION
/// ==========================
/// 
/// Description
/// -----------
/// 
/// The `FIT_STATIC_LAMBDA_FUNCTION` macro allows initializes a global function
/// object that contains non-capturing lambdas. It also ensures that the global
/// function object has a unique address across translation units. This helps
/// prevent possible ODR-violations.
/// 
/// Example
/// -------
/// 
///     FIT_STATIC_LAMBDA_FUNCTION(add_one) = [](int x)
///     {
///         return x + 1;
///     };
/// 

#include <fit/function.h>
#include <type_traits>
#include <utility>
#include <fit/returns.h>
#include <fit/reveal.h>
#include <fit/detail/static_constexpr.h>
#include <fit/detail/static_const_var.h>

#define FIT_CONST_FOLD(x) (__builtin_constant_p(x) ? (x) : (x))

#ifndef FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR
#define FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR 0
#endif

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

template<class T>
struct reveal_static_function_wrapper_factor
{
#if FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR
    template<class F>
    constexpr reveal_adaptor<static_function_wrapper<F>> operator += (F*)
    {
        return {};
    }
#else
    template<class F>
    constexpr const reveal_adaptor<F>& operator += (F*)
    {
        static_assert(std::is_empty<F>::value, "Function or lambda expression must be empty");
        return FIT_CONST_FOLD(reinterpret_cast<const reveal_adaptor<F>&>(static_const_var<T>()));
    }
#endif
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

#if FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR
#define FIT_DETAIL_STATIC_FUNCTION_AUTO FIT_STATIC_CONSTEXPR auto
#else
#define FIT_DETAIL_STATIC_FUNCTION_AUTO FIT_STATIC_AUTO_REF
#endif

#define FIT_DETAIL_MAKE_STATIC fit::detail::static_function_wrapper_factor() += true ? nullptr : fit::detail::static_addr()
#define FIT_DETAIL_MAKE_REVEAL_STATIC(T) fit::detail::reveal_static_function_wrapper_factor<T>() += true ? nullptr : fit::detail::static_addr()
#define FIT_STATIC_LAMBDA_FUNCTION(name) \
struct fit_private_static_function_ ## name {}; \
FIT_DETAIL_STATIC_FUNCTION_AUTO name = FIT_DETAIL_MAKE_REVEAL_STATIC(fit_private_static_function_ ## name)

#define FIT_STATIC_LAMBDA FIT_DETAIL_MAKE_STATIC = []


#endif