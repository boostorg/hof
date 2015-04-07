/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    result.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_RESULT_H
#define FIT_GUARD_RESULT_H

/// result
/// ======
/// 
/// Description
/// -----------
/// 
/// The `result` function adaptor sets the return type for the function.
/// 
/// Synopsis
/// --------
/// 
///     template<class Result, class F>
///     constexpr result_adaptor<Result, F> result(F f);
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
///     FunctionObject
///     MoveConstructible
/// 
/// Example
/// -------
/// 
///     struct id
///     {
///         template<class T>
///         T operator()(T x) const
///         {
///             return x;
///         }
///     };
/// 
///     auto int_result = fit::result<int>(id());
///     static_assert(std::is_same<decltype(int_result(true)), int>::value, "Not the same type");
/// 

#include <fit/detail/delegate.h>
#include <fit/is_callable.h>
#include <fit/always.h>
#include <fit/reveal.h>

namespace fit {

template<class Result, class F>
struct result_adaptor : F
{
    FIT_INHERIT_CONSTRUCTOR(result_adaptor, F)

    typedef Result result_type;

    struct failure
    : failure_for<F>
    {};

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Ts, class=typename std::enable_if<(fit::is_callable<F(Ts...)>::value)>::type>
    constexpr result_type operator()(Ts&&... xs) const
    {
        return this->base_function(xs...)(fit::forward<Ts>(xs)...);
    };
};

template<class F>
struct result_adaptor<void, F> : F
{
    FIT_INHERIT_CONSTRUCTOR(result_adaptor, F)

    typedef void result_type;

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Ts, class=typename std::enable_if<(fit::is_callable<F(Ts...)>::value)>::type>
    constexpr result_type operator()(Ts&&... xs) const
    {
        this->base_function(xs...)(fit::forward<Ts>(xs)...);
    };
};

// TODO: Make this a variable template in C++14
template<class Result, class F>
constexpr result_adaptor<Result, F> result(F f)
{
    return result_adaptor<Result, F>(std::move(f));
}

}

#endif