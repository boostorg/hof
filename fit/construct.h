/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    construct.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_CONSTRUCT_H
#define FIT_GUARD_CONSTRUCT_H

#include <fit/detail/forward.h>
#include <fit/detail/delegate.h>
#include <fit/detail/join.h>

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
