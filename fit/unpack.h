/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    unpack.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_UNPACK_H
#define FIT_GUARD_UNPACK_H

/// unpack
/// ======
/// 
/// Description
/// -----------
/// 
/// The `unpack` function adaptor takes a sequence and uses the elements of
/// the sequence for the arguments to the function. Multiple sequences can be
/// passed to the function. All elements from each sequence will be passed
/// into the function. 
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     unpack_adaptor<F> unpack(F f);
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
///     struct sum
///     {
///         template<class T, class U>
///         T operator()(T x, U y) const
///         {
///             return x+y;
///         }
///     };
/// 
///     int r = unpack(sum())(std::make_tuple(3,2));
///     assert(r == 5);
/// 
/// 
/// unpack_sequence
/// ===============
/// 
/// How to unpack a sequence can be defined by specializing `unpack_sequence`.
/// By default, `std::tuple` can be used with unpack.
/// 
/// Synopsis
/// --------
/// 
///     template<class Sequence, class=void>
///     struct unpack_sequence;
/// 
/// Example
/// -------
/// 
///     template<class... Ts>
///     struct unpack_sequence<my_sequence<Ts...>>
///     {
///         template<class F, class Sequence>
///         constexpr static auto apply(F&& f, Sequence&& s) FIT_RETURNS
///         (
///             s(std::forward<F>(f))
///         );
///     };
/// 

#include <fit/returns.h>
#include <tuple>
#include <fit/detail/seq.h>
#include <fit/capture.h>
#include <fit/always.h>
#include <fit/detail/delegate.h>
#include <fit/detail/move.h>
#include <fit/detail/make.h>
#include <fit/detail/static_constexpr.h>

namespace fit {

template<class Sequence, class=void>
struct unpack_sequence;

namespace detail {

template<class F, class Sequence>
constexpr auto unpack_impl(F&& f, Sequence&& s) FIT_RETURNS
(
    fit::unpack_sequence<typename std::remove_cv<typename std::remove_reference<Sequence>::type>::type>::
            apply(fit::forward<F>(f), fit::forward<Sequence>(s))
);

template<class F, class... Sequences>
constexpr auto unpack_join(F&& f, Sequences&&... s) FIT_RETURNS
(
    unpack_impl(fit::forward<F>(f), fit::pack_join(unpack_impl(fit::pack_forward, fit::forward<Sequences>(s))...))
);

}

template<class F>
struct unpack_adaptor : F
{
    FIT_INHERIT_CONSTRUCTOR(unpack_adaptor, F);

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(unpack_adaptor);

    template<class T>
    constexpr auto operator()(T&& x) const
    FIT_RETURNS
    (
        detail::unpack_impl(FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(x)), fit::forward<T>(x))
    );

    template<class T, class... Ts>
    constexpr auto operator()(T&& x, Ts&&... xs) const FIT_RETURNS
    (
        detail::unpack_join(FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(x)), fit::forward<T>(x), fit::forward<Ts>(xs)...)
    );
};

FIT_STATIC_CONSTEXPR detail::make<unpack_adaptor> unpack = {};

namespace detail {

template<class Sequence>
constexpr typename gens<std::tuple_size<Sequence>::value>::type 
make_tuple_gens(const Sequence&)
{
    return {};
}

template<class F, class T, int ...N>
constexpr auto unpack_tuple(F&& f, T && t, seq<N...>) FIT_RETURNS
(
    f(FIT_AUTO_FORWARD(std::get<N>(t))...)
);

}

template<class... Ts>
struct unpack_sequence<std::tuple<Ts...>>
{
    template<class F, class S>
    constexpr static auto apply(F&& f, S&& t) FIT_RETURNS
    (
        detail::unpack_tuple(fit::forward<F>(f), fit::forward<S>(t), detail::make_tuple_gens(t))
    );
};

template<class T, class... Ts>
struct unpack_sequence<detail::pack_base<T, Ts...>>
{
    template<class F, class P>
    constexpr static auto apply(F&& f, P&& p) FIT_RETURNS
    (
        p(fit::forward<F>(f))
    );
};

}

#endif