/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    construct.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_CONSTRUCT_H
#define FIT_GUARD_CONSTRUCT_H

/// construct
/// =========
/// 
/// Description
/// -----------
/// 
/// The `construct` function returns a function object that will construct the
/// object when the called. A template can also be givien, which it will
/// deduce the parameters to the template.
/// 
/// Synopsis
/// --------
/// 
///     template<class T>
///     constexpr auto construct();
/// 
///     template<template<class...> class Template>
///     constexpr auto construct();
/// 
/// Requirements
/// ------------
/// 
/// T or Template must be:
/// 
///     MoveConstructible
/// 
/// Example
/// -------
/// 
///     auto v = fit::construct<std::vector<int>>()(5, 5);
///     assert(v.size() == 5);
/// 

#include <fit/detail/forward.h>
#include <fit/detail/move.h>
#include <fit/detail/delegate.h>
#include <fit/detail/join.h>

#include <initializer_list>

namespace fit { namespace detail {

template<class T>
struct construct_f
{
    constexpr construct_f()
    {}
    template<class... Ts, FIT_ENABLE_IF_CONSTRUCTIBLE(T, Ts...)>
    constexpr T operator()(Ts&&... xs) const
    {
        return T(fit::forward<Ts>(xs)...);
    }

    template<class X, FIT_ENABLE_IF_CONSTRUCTIBLE(T, std::initializer_list<X>&&)>
    constexpr T operator()(std::initializer_list<X>&& x) const
    {
        return T(fit::move(x));
    }

    template<class X, FIT_ENABLE_IF_CONSTRUCTIBLE(T, std::initializer_list<X>&)>
    constexpr T operator()(std::initializer_list<X>& x) const
    {
        return T(x);
    }

    template<class X, FIT_ENABLE_IF_CONSTRUCTIBLE(T, const std::initializer_list<X>&)>
    constexpr T operator()(const std::initializer_list<X>& x) const
    {
        return T(x);
    }
};

template<template<class...> class Template>
struct construct_template_f
{
    constexpr construct_template_f()
    {}
    template<class... Ts, FIT_ENABLE_IF_CONSTRUCTIBLE(FIT_JOIN(Template, Ts...), Ts...)>
    constexpr FIT_JOIN(Template, Ts...) operator()(Ts&&... xs) const
    {
        return FIT_JOIN(Template, Ts...)(fit::forward<Ts>(xs)...);
    }
};
}

template<class T>
constexpr detail::construct_f<T> construct()
{
    return {};
}

template<template<class...> class Template>
constexpr detail::construct_template_f<Template> construct()
{
    return {};
}

}

#endif
