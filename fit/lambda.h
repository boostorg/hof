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
/// The `FIT_STATIC_LAMBDA_FUNCTION` macro allows initializing a global
/// function object that contains non-capturing lambdas. It also ensures that
/// the global function object has a unique address across translation units.
/// This helps prevent possible ODR-violations.
/// 
/// Example
/// -------
/// 
///     FIT_STATIC_LAMBDA_FUNCTION(add_one) = [](int x)
///     {
///         return x + 1;
///     };
/// 

#include <type_traits>
#include <utility>
#include <fit/detail/result_of.h>
#include <fit/reveal.h>
#include <fit/detail/static_constexpr.h>
#include <fit/detail/static_const_var.h>


// #define FIT_HAS_STATIC_LAMBDA 1
#ifndef FIT_HAS_STATIC_LAMBDA
#ifdef _MSC_VER
#define FIT_HAS_STATIC_LAMBDA 0
#else
#define FIT_HAS_STATIC_LAMBDA 1
#endif
#endif

#ifndef FIT_REWRITE_STATIC_LAMBDA
#ifdef _MSC_VER
#define FIT_REWRITE_STATIC_LAMBDA 1
#else
#define FIT_REWRITE_STATIC_LAMBDA 0
#endif
#endif

#ifndef FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR
#if defined(_MSC_VER) || FIT_REWRITE_STATIC_LAMBDA
#define FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR 1
#else
#define FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR 0
#endif
#endif

namespace fit {

namespace detail {

struct constexpr_deduce
{
    constexpr constexpr_deduce()
    {}
    template<class F>
    constexpr operator F() const
    {
        return F();
    }
};

template<class T>
struct constexpr_deduce_unique
{
    constexpr constexpr_deduce_unique()
    {}
#if FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR
    template<class F>
    constexpr operator F() const
    {
        // static_assert(std::is_default_constructible<F>::value, "Function not default constructible");
        return F();
    }
#else
    template<class F>
    constexpr operator const F&() const
    {
        static_assert(std::is_empty<F>::value, "Function or lambda expression must be empty");
        return FIT_CONST_FOLD(reinterpret_cast<const F&>(static_const_var<T>()));
    }
#endif
};

template<class F>
struct static_function_wrapper
{
    // Default constructor necessary for MSVC
    constexpr static_function_wrapper()
    {}

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
    FIT_SFINAE_RESULT(const F&, id_<Ts>...) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        FIT_RETURNS_REINTERPRET_CAST(const F&)(*FIT_CONST_THIS)(fit::forward<Ts>(xs)...)
    );
};

struct static_function_wrapper_factor
{
    constexpr static_function_wrapper_factor()
    {}
    template<class F>
    constexpr static_function_wrapper<F> operator= (const F&) const
    {
        // static_assert(std::is_literal_type<static_function_wrapper<F>>::value, "Function wrapper not a literal type");
        return {};
    }
};

#if FIT_REWRITE_STATIC_LAMBDA
template<class T, class=void>
struct is_rewritable
: std::false_type
{};

template<class T>
struct is_rewritable<T, typename detail::holder<
    typename T::fit_rewritable_tag
>::type>
: std::is_same<typename T::fit_rewritable_tag, T>
{};

template<class T, class=void>
struct is_rewritable1
: std::false_type
{};

template<class T>
struct is_rewritable1<T, typename detail::holder<
    typename T::fit_rewritable1_tag
>::type>
: std::is_same<typename T::fit_rewritable1_tag, T>
{};


template<class T, class=void>
struct rewrite_lambda;

template<template<class...> class Adaptor, class... Ts>
struct rewrite_lambda<Adaptor<Ts...>, typename std::enable_if<
    is_rewritable<Adaptor<Ts...>>::value
>::type>
{
    typedef Adaptor<typename rewrite_lambda<Ts>::type...> type;
};

template<template<class...> class Adaptor, class T, class... Ts>
struct rewrite_lambda<Adaptor<T, Ts...>, typename std::enable_if<
    is_rewritable1<Adaptor<T, Ts...>>::value
>::type>
{
    typedef Adaptor<typename rewrite_lambda<T>::type, Ts...> type;
};

template<class T>
struct rewrite_lambda<T, typename std::enable_if<
    std::is_empty<T>::value && 
    !is_rewritable<T>::value && 
    !is_rewritable1<T>::value
>::type>
{
    typedef static_function_wrapper<T> type;
};

template<class T>
struct rewrite_lambda<T, typename std::enable_if<
    !std::is_empty<T>::value && 
    !is_rewritable<T>::value && 
    !is_rewritable1<T>::value
>::type>
{
    typedef T type;
};

#endif

struct reveal_static_lambda_function_wrapper_factor
{
    constexpr reveal_static_lambda_function_wrapper_factor()
    {}
#if FIT_REWRITE_STATIC_LAMBDA
    template<class T, class F=typename rewrite_lambda<F>::type>
    constexpr reveal_adaptor<F> 
    operator=(const T&) const
    {
        // static_assert(std::is_default_constructible<F>::value, "Rewritten lambda not default constructible");
        // static_assert(std::is_literal_type<F>::value, "Rewritten lambda is not a literal type");
        return reveal_adaptor<F>();
    }
#elif FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR
    template<class F>
    constexpr reveal_adaptor<static_function_wrapper<F>> operator=(const F&) const
    {
        return {};
    }
#else
    template<class F>
    constexpr reveal_adaptor<F> operator=(const F& f) const
    {
        return reveal_adaptor<F>(f);
    }
#endif
};

}}

#if FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR || FIT_REWRITE_STATIC_LAMBDA
#define FIT_DETAIL_STATIC_FUNCTION_AUTO FIT_STATIC_CONSTEXPR auto
#else
#define FIT_DETAIL_STATIC_FUNCTION_AUTO FIT_STATIC_AUTO_REF
#endif

#define FIT_DETAIL_MAKE_STATIC true ? fit::detail::constexpr_deduce() : fit::detail::static_function_wrapper_factor()
#define FIT_DETAIL_MAKE_REVEAL_STATIC(T) true ? fit::detail::constexpr_deduce_unique<T>() : fit::detail::reveal_static_lambda_function_wrapper_factor()
#define FIT_STATIC_LAMBDA_FUNCTION(name) \
struct fit_private_static_function_ ## name {}; \
FIT_DETAIL_STATIC_FUNCTION_AUTO name = FIT_DETAIL_MAKE_REVEAL_STATIC(fit_private_static_function_ ## name)

#define FIT_STATIC_LAMBDA FIT_DETAIL_MAKE_STATIC = []


#endif