/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    implicit.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_IMPLICIT_H
#define FIT_GUARD_FUNCTION_IMPLICIT_H

/// implicit
/// ========
/// 
/// Description
/// -----------
/// 
/// The `implicit` adaptor is a static function adaptor that uses the type
/// that the return value can be converted to in order to determine the type
/// of the template parameter. In essence, it will deduce the type for the
/// template parameter using the type of variable the result is assigned to.
/// Since it is static function adaptor, the function must be default
/// constructible.
/// 
/// Synopsis
/// --------
/// 
///     template<template <class...> class F>
///     class implicit<F>;
/// 
/// Example
/// -------
/// 
///     template<class T>
///     struct auto_caster
///     {
///         template<class U>
///         T operator()(U x)
///         {
///             return T(x);
///         }
///     };
/// 
///     implicit<auto_caster> auto_cast = {};
/// 
///     struct auto_caster_foo
///     {
///         int i;
///         explicit auto_caster_foo(int i) : i(i) {}
/// 
///     };
/// 
///     float f = 1.5;
///     int i = auto_cast(f);
///     auto_caster_foo x = auto_cast(1);
///     assert(1 == i);
///     assert(1 == x.i);
/// 

#include <fit/invoke.h>
#include <fit/detail/ref_tuple.h>

namespace fit { namespace detail {

template<class F, class Seq, class X, class=void>
struct is_implicit_callable
: std::false_type
{};

template<class F, class Seq, class X>
struct is_implicit_callable<F, Seq, X, typename std::enable_if<
    std::is_convertible<decltype(invoke(std::declval<F>(), std::declval<Seq>())), X>::value
>::type>
: std::true_type
{};

template<template <class...> class F, class Sequence>
struct implicit_invoke
{
    Sequence seq;

    constexpr implicit_invoke(Sequence seq) : seq(seq)
    {}

    template<class X, class=typename std::enable_if<is_implicit_callable<F<X>, Sequence, X>::value>::type>
    constexpr operator X() const
    {
        return invoke(F<X>(), seq);
    }

    template<template <class...> class F2, class Sequence2>
    constexpr operator implicit_invoke<F2, Sequence2>() const
    {
        return implicit_invoke<F2, Sequence2>(seq);
    }
};

template<template <class> class F, class Sequence>
constexpr implicit_invoke<F, Sequence> make_implicit_invoke(Sequence&& seq)
{
    return implicit_invoke<F, Sequence>(std::forward<Sequence>(seq));
}

}


template<template <class> class F>
struct implicit
{
    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const
    {
        return detail::make_implicit_invoke<F>(detail::make_ref_tuple(std::forward<Ts>(xs)...));
    }

};

}

#endif
