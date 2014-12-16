/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    reveal.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_REVEAL_H
#define FIT_GUARD_FUNCTION_REVEAL_H

/// reveal
/// ======
/// 
/// Description
/// -----------
/// 
/// The `reveal` function adaptor turns substitution failures into compile errors.
/// Sometimes an error in a function that causes a substitution failure, will
/// remove the function from valid overloads thus masking the error inside the
/// function. The `reveal` adaptor reveals these errors by forcing a compile
/// error, instead of a substitution failure.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     reveal_adaptor<F> reveal(F f);
/// 
/// Requirements
/// ------------
/// 
/// Fs must be:
/// 
///     FunctionObject
///     MoveConstructible
/// 

#include <fit/always.h>
#include <fit/returns.h>
#include <fit/is_callable.h>
#include <fit/detail/move.h>
#include <fit/detail/delegate.h>

namespace fit { 

namespace detail {
template<template<class...> class T>
struct template_holder
{
    typedef void type;
};

template<class T, class=void>
struct has_failure
: std::false_type
{};

template<class T>
struct has_failure<T, typename template_holder<
    T::template failure
>::type>
: std::true_type
{};

template<class Sig>
struct failure_check;

template<class F, class... Ts>
struct failure_check<F(Ts...)>
{
    // Use virtual function to reduce backtrace
    virtual void check()
    {
        typedef decltype(std::declval<F>()(std::declval<Ts>()...)) type;
    }
    // typedef decltype(std::declval<F>()(std::declval<Ts>()...)) type;
};

template<class... Ts>
struct failures
: Ts...
{};

template<class... Ts>
struct failure_checks
{
    typedef failures<Ts...> type;
};

template<class Sig, class Enable = void>
struct failure_for_;

template<class F, class... Ts>
struct failure_for_<F(Ts...), typename std::enable_if<has_failure<F>::value>::type>
: F::template failure<Ts...>
{};

template<class F, class... Ts>
struct failure_for_<F(Ts...), typename std::enable_if<!has_failure<F>::value>::type>
{
    typedef failure_check<F(Ts...)> type;
};
}

template<class... Ts>
struct failure_for
: detail::failure_checks<typename detail::failure_for_<Ts>::type...>
{};

template<class F>
struct reveal_adaptor: F
{

    FIT_INHERIT_CONSTRUCTOR(reveal_adaptor, F);

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(reveal_adaptor);
    
    template<class... Ts>
    constexpr auto operator()(Ts && ... xs) const
    FIT_RETURNS(FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...))(fit::forward<Ts>(xs)...));

    struct fail {};

    template<class... Ts>
    typename std::enable_if<
        !is_callable<F(Ts&&...)>::value
    >::type operator()(Ts&&...) const
    {
        typename failure_for<F(Ts&&...)>::type();
    }

};

template<class F>
constexpr reveal_adaptor<F> reveal(F f)
{
    return reveal_adaptor<F>(fit::move(f));
}

}

#endif
