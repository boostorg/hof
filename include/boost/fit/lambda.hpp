/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    lambda.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FUNCTION_LAMBDA_H
#define BOOST_FIT_GUARD_FUNCTION_LAMBDA_H

/// BOOST_FIT_STATIC_LAMBDA
/// =================
/// 
/// Description
/// -----------
/// 
/// The `BOOST_FIT_STATIC_LAMBDA` macro allows initializing non-capturing lambdas at
/// compile-time in a `constexpr` expression.
/// 
/// Example
/// -------
/// 
///     const constexpr auto add_one = BOOST_FIT_STATIC_LAMBDA(int x)
///     {
///         return x + 1;
///     };
///     assert(3 == add_one(2));
/// 
/// BOOST_FIT_STATIC_LAMBDA_FUNCTION
/// ==========================
/// 
/// Description
/// -----------
/// 
/// The `BOOST_FIT_STATIC_LAMBDA_FUNCTION` macro allows initializing a global
/// function object that contains non-capturing lambdas. It also ensures that
/// the global function object has a unique address across translation units.
/// This helps prevent possible ODR-violations.
/// 
/// By default, all functions defined with `BOOST_FIT_STATIC_LAMBDA_FUNCTION` use
/// the `boost::fit::reveal` adaptor to improve error messages.
/// 
/// Example
/// -------
/// 
///     BOOST_FIT_STATIC_LAMBDA_FUNCTION(add_one) = [](int x)
///     {
///         return x + 1;
///     };
///     assert(3 == add_one(2));
/// 

#include <type_traits>
#include <utility>
#include <boost/fit/detail/result_of.hpp>
#include <boost/fit/reveal.hpp>
#include <boost/fit/detail/constexpr_deduce.hpp>
#include <boost/fit/detail/static_constexpr.hpp>
#include <boost/fit/detail/static_const_var.hpp>


#define BOOST_FIT_HAS_STATIC_LAMBDA 1

#ifndef BOOST_FIT_REWRITE_STATIC_LAMBDA
#ifdef _MSC_VER
#define BOOST_FIT_REWRITE_STATIC_LAMBDA 1
#else
#define BOOST_FIT_REWRITE_STATIC_LAMBDA 0
#endif
#endif

namespace boost { namespace fit {

namespace detail {

template<class F>
struct static_function_wrapper
{
    // Default constructor necessary for MSVC
    constexpr static_function_wrapper()
    {}

    static_assert(BOOST_FIT_IS_EMPTY(F), "Function or lambda expression must be empty");

    struct failure
    : failure_for<F>
    {};

    template<class... Ts>
    const F& base_function(Ts&&...) const
    {
        return reinterpret_cast<const F&>(*this);
    }

    BOOST_FIT_RETURNS_CLASS(static_function_wrapper);

    template<class... Ts>
    BOOST_FIT_SFINAE_RESULT(const F&, id_<Ts>...) 
    operator()(Ts&&... xs) const BOOST_FIT_SFINAE_RETURNS
    (
        BOOST_FIT_RETURNS_REINTERPRET_CAST(const F&)(*BOOST_FIT_CONST_THIS)(BOOST_FIT_FORWARD(Ts)(xs)...)
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

#if BOOST_FIT_REWRITE_STATIC_LAMBDA
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

template<class T>
struct reveal_static_lambda_function_wrapper_factor
{
    constexpr reveal_static_lambda_function_wrapper_factor()
    {}
#if BOOST_FIT_REWRITE_STATIC_LAMBDA
    template<class F>
    constexpr reveal_adaptor<typename rewrite_lambda<F>::type> 
    operator=(const F&) const
    {
        return reveal_adaptor<typename rewrite_lambda<F>::type>();
    }
#elif BOOST_FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR
    template<class F>
    constexpr reveal_adaptor<static_function_wrapper<F>> operator=(const F&) const
    {
        return {};
    }
#else
    template<class F>
    constexpr const reveal_adaptor<F>& operator=(const F&) const
    {
        return reinterpret_cast<const reveal_adaptor<F>&>(static_const_var<T>());
    }
#endif
};

}}} // namespace boost::fit

#if BOOST_FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR || BOOST_FIT_REWRITE_STATIC_LAMBDA
#define BOOST_FIT_DETAIL_STATIC_FUNCTION_AUTO BOOST_FIT_STATIC_CONSTEXPR auto
#else
#define BOOST_FIT_DETAIL_STATIC_FUNCTION_AUTO BOOST_FIT_STATIC_AUTO_REF
#endif

#define BOOST_FIT_DETAIL_MAKE_STATIC BOOST_FIT_DETAIL_CONSTEXPR_DEDUCE boost::fit::detail::static_function_wrapper_factor()
#define BOOST_FIT_DETAIL_MAKE_REVEAL_STATIC(T) BOOST_FIT_DETAIL_CONSTEXPR_DEDUCE_UNIQUE(T) boost::fit::detail::reveal_static_lambda_function_wrapper_factor<T>()
#define BOOST_FIT_STATIC_LAMBDA_FUNCTION(name) \
struct fit_private_static_function_ ## name {}; \
BOOST_FIT_DETAIL_STATIC_FUNCTION_AUTO name = BOOST_FIT_DETAIL_MAKE_REVEAL_STATIC(fit_private_static_function_ ## name)

#define BOOST_FIT_STATIC_LAMBDA BOOST_FIT_DETAIL_MAKE_STATIC = []


#endif
