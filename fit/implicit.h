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
/// Requirements
/// ------------
/// 
/// F must be a template class, that is a:
/// 
///     FunctionObject
///     DefaultConstructible
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
#include <fit/pack.h>
#include <fit/returns.h>

namespace fit { namespace detail {

template<class F, class Pack, class X, class=void>
struct is_implicit_callable
: std::false_type
{};

template<class F, class Pack, class X>
struct is_implicit_callable<F, Pack, X, typename std::enable_if<
    std::is_convertible<decltype(std::declval<Pack>()(std::declval<F>())), X>::value
>::type>
: std::true_type
{};

template<template <class...> class F, class Pack>
struct implicit_invoke
{
    Pack p;

    constexpr implicit_invoke(Pack p) : p(p)
    {}

    template<class X, class=typename std::enable_if<is_implicit_callable<F<X>, Pack, X>::value>::type>
    constexpr operator X() const
    {
        return p(F<X>());
    }

    template<template <class...> class F2, class Pack2>
    constexpr operator implicit_invoke<F2, Pack2>() const
    {
        return implicit_invoke<F2, Pack2>(p);
    }
};

template<template <class...> class F, class Pack>
constexpr implicit_invoke<F, Pack> make_implicit_invoke(Pack&& p)
{
    return implicit_invoke<F, Pack>(std::forward<Pack>(p));
}

}


template<template <class...> class F>
struct implicit
{
    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        detail::make_implicit_invoke<F>(fit::pack(std::forward<Ts>(xs)...))
    );

};

}

#endif
