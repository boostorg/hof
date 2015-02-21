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
#include <fit/identity.h>
#include <fit/detail/move.h>
#include <fit/detail/delegate.h>
#include <fit/detail/holder.h>

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
struct has_failure<T, typename holder<
    typename T::failure
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
}

template<class F, class=void>
struct get_failure
{
    template<class... Ts>
    struct of
    {
        template<class Id>
        using apply = decltype(Id()(std::declval<F>())(std::declval<Ts>()...));
    };
};

template<class F>
struct get_failure<F, typename std::enable_if<detail::has_failure<F>::value>::type>
: F::failure
{};

namespace detail {
template<class Failure, class... Ts>
struct apply_failure
: Failure::template of<Ts...>
{};

template<class F, class Failure>
struct reveal_failure
{
    // This is just a placeholder to produce a note in the compiler, it is
    // never called
    template<
        class... Ts, 
        class=typename std::enable_if<(!is_callable<F(Ts...)>::value)>::type
    >
    constexpr auto operator()(Ts&&... xs) -> 
        typename apply_failure<Failure, Ts...>::template apply<decltype(identity)>;
};

template<class F, class Failure=get_failure<F>, class=void>
struct traverse_failure 
: reveal_failure<F, Failure>
{};

template<class F, class Failure>
struct traverse_failure<F, Failure, typename holder< 
    typename Failure::children
>::type> 
: Failure::children::template overloads<F>
{};

template<class Failure, class Transform, class=void>
struct transform_failures 
: Transform::template apply<Failure>
{};

template<class Failure, class Transform>
struct transform_failures<Failure, Transform, typename holder< 
    typename Failure::children
>::type> 
: Failure::children::template transform<Transform>
{};

}

template<class Failure, class... Failures>
struct failures;

template<class... Fs>
struct with_failures
{
    using children = failures<Fs...>;
};

template<class Failure, class... Failures>
struct failures 
{
    template<class Transform>
    struct transform
    : with_failures<detail::transform_failures<Failure, Transform>, detail::transform_failures<Failures, Transform>...>
    {};
    template<class F>
    struct overloads
    : detail::traverse_failure<F, Failure>, failures<Failures...>::template overloads<F>
    {
        using detail::traverse_failure<F, Failure>::operator();
        using failures<Failures...>::template overloads<F>::operator();
    };
};

template<class Failure>
struct failures<Failure>
{
    template<class Transform>
    struct transform
    : with_failures<detail::transform_failures<Failure, Transform>>
    {};
    template<class F>
    struct overloads
    : detail::traverse_failure<F, Failure>
    {};
};

template<class Transform, class... Fs>
struct failure_map
: with_failures<detail::transform_failures<get_failure<Fs>, Transform>...>
{};

template<class... Fs>
struct failure_for
: with_failures<get_failure<Fs>...>
{};

template<class F>
struct reveal_adaptor
: detail::traverse_failure<F>, F
{
    using detail::traverse_failure<F>::operator();
    using F::operator();

    FIT_INHERIT_CONSTRUCTOR(reveal_adaptor, F);
};

template<class F>
constexpr reveal_adaptor<F> reveal(F f)
{
    return reveal_adaptor<F>(fit::move(f));
}

}

#endif
