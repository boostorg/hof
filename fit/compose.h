/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    compose.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_COMPOSE_H
#define FIT_GUARD_FUNCTION_COMPOSE_H

/// compose
/// =======
/// 
/// Description
/// -----------
/// 
/// The `compose` function adaptor provides function composition. It produces
/// a function object that composes a set of functions, ie the output of one
/// function becomes the input of the second function. So, `compose(f, g)(0)`
/// is equivalent to `f(g(0))`.
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class... Fs>
///     constexpr compose_adaptor<Fs...> compose(Fs... fs);
/// 
/// Requirements
/// ------------
/// 
/// Fs must be:
/// 
///     FunctionObject
///     MoveConstructible
/// 
/// Example
/// -------
/// 
///     struct increment
///     {
///         template<class T>
///         T operator()(T x) const
///         {
///             return x + 1;
///         }
///     };
/// 
///     struct decrement
///     {
///         template<class T>
///         T operator()(T x) const
///         {
///             return x - 1;
///         }
///     };
/// 
///     int r = compose(increment(), decrement(), increment())(3);
///     assert(r == 4);
/// 

#include <fit/returns.h>
#include <fit/always.h>
#include <fit/detail/delegate.h>
#include <fit/detail/join.h>
#include <tuple>

namespace fit { namespace detail {

// TODO: Try to use inheritance but make each base class unique
template<class F1, class F2>
struct compose_kernel
{
    F1 f1;
    F2 f2;
    constexpr compose_kernel()
    {}

    template<class A, class B>
    constexpr compose_kernel(A&& f1, B&& f2) 
    : f1(std::forward<A>(f1)), f2(std::forward<B>(f2))
    {}

    FIT_RETURNS_CLASS(compose_kernel);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        FIT_CONST_THIS->f1(FIT_CONST_THIS->f2(std::forward<Ts>(xs)...))
    );
};
}

template<class F, class... Fs>
struct compose_adaptor : detail::compose_kernel<F, FIT_JOIN(compose_adaptor, Fs...)>
{
    typedef FIT_JOIN(compose_adaptor, Fs...) tail;
    typedef detail::compose_kernel<F, tail> base;

    constexpr compose_adaptor() {}

    template<class X, class... Xs, FIT_ENABLE_IF_CONVERTIBLE(X, F), FIT_ENABLE_IF_CONSTRUCTIBLE(tail, Xs...)>
    constexpr compose_adaptor(X&& f1, Xs&& ... fs) 
    : base(std::forward<X>(f1), tail(std::forward<Xs>(fs)...))
    {}
};

template<class F>
struct compose_adaptor<F> : F
{
    constexpr compose_adaptor() {}

    template<class X, FIT_ENABLE_IF_CONVERTIBLE(X, F)>
    constexpr compose_adaptor(X&& f1) 
    : F(std::forward<X>(f1))
    {}

};

template<class... Fs>
constexpr FIT_JOIN(compose_adaptor, Fs...) compose(Fs... fs)
{
    return FIT_JOIN(compose_adaptor, Fs...)(std::move(fs)...);
}

}

#endif
