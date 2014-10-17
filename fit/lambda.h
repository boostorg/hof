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
/// The `FIT_STATIC_LAMBDA` macro allows initializing lambdas at compile-time
/// in a `constexpr` expression.
/// 
/// Example
/// -------
/// 
///     const constexpr auto add_one = FIT_STATIC_LAMBDA(int x)
///     {
///         return x + 1;
///     };
/// 


#include <type_traits>
#include <utility>
#include <fit/returns.h>

namespace fit {

namespace detail {
template<class F>
struct lambda_wrapper
{
    static_assert(std::is_empty<F>::value, "Lambdas must be empty");

    FIT_RETURNS_CLASS(lambda_wrapper);

    template<class... Ts>
    auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        FIT_RETURNS_REINTERPRET_CAST(const F&)(*FIT_CONST_THIS)(std::forward<Ts>(xs)...)
    );
};

struct lambda_wrapper_factor
{
    template<class F>
    constexpr lambda_wrapper<F> operator += (F*)
    {
        return {};
    }
};

struct lambda_addr
{
    template<class T>
    friend typename std::remove_reference<T>::type *operator+(lambda_addr, T &&t) 
    {
        return &t;
    }
};

}

#define FIT_STATIC_LAMBDA fit::detail::lambda_wrapper_factor() += true ? nullptr : fit::detail::lambda_addr() + []

}

#endif