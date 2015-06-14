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

#include <fit/detail/result_of.h>
#include <fit/always.h>
#include <fit/detail/delegate.h>
#include <fit/detail/compressed_pair.h>
#include <fit/detail/join.h>
#include <tuple>
#include <fit/detail/move.h>
#include <fit/detail/make.h>
#include <fit/detail/static_const_var.h>

namespace fit { namespace detail {

template<class F1, class F2>
struct compose_kernel : detail::compressed_pair<F1, F2>
{
    typedef detail::compressed_pair<F1, F2> base_type;

    FIT_INHERIT_CONSTRUCTOR(compose_kernel, base_type)

    FIT_RETURNS_CLASS(compose_kernel);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(const F1&, result_of<const F2&, id_<Ts>...>) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        FIT_MANGLE_CAST(const F1&)(FIT_CONST_THIS->first(xs...))(
            FIT_MANGLE_CAST(const F2&)(FIT_CONST_THIS->second(xs...))(fit::forward<Ts>(xs)...)
        )
    );
};
}

template<class F, class... Fs>
struct compose_adaptor : detail::compose_kernel<F, FIT_JOIN(compose_adaptor, Fs...)>
{
    typedef compose_adaptor fit_rewritable_tag;
    typedef FIT_JOIN(compose_adaptor, Fs...) tail;
    typedef detail::compose_kernel<F, tail> base_type;

    FIT_INHERIT_DEFAULT(compose_adaptor, base_type)

    template<class X, class... Xs, FIT_ENABLE_IF_CONVERTIBLE(X, F), FIT_ENABLE_IF_CONSTRUCTIBLE(tail, Xs...)>
    constexpr compose_adaptor(X&& f1, Xs&& ... fs) 
    : base_type(fit::forward<X>(f1), tail(fit::forward<Xs>(fs)...))
    {}
};

template<class F>
struct compose_adaptor<F> : F
{
    typedef compose_adaptor fit_rewritable_tag;

    FIT_INHERIT_DEFAULT(compose_adaptor, F)

    template<class X, FIT_ENABLE_IF_CONVERTIBLE(X, F)>
    constexpr compose_adaptor(X&& f1) 
    : F(fit::forward<X>(f1))
    {}

};

FIT_DECLARE_STATIC_VAR(compose, detail::make<compose_adaptor>);

}

#endif
