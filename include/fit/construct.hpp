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
/// object when the called. A template can also be givien, which it will deduce
/// the parameters to the template. The `construct_meta` can be used to
/// construct the object from a metafunction.
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
///     template<class MetafunctionClass>
///     constexpr auto construct_meta();
/// 
///     template<template<class...> class MetafunctionTemplate>
///     constexpr auto construct_meta();
/// 
/// Semantics
/// ---------
/// 
///     assert(construct<T>()(xs...) == T(xs...));
///     assert(construct<Template>()(xs...) == Template<decltype(xs)...>(xs...));
///     assert(construct_meta<MetafunctionClass>()(xs...) == MetafunctionClass::apply<decltype(xs)...>(xs...));
///     assert(construct_meta<MetafunctionTemplate>()(xs...) == MetafunctionTemplate<decltype(xs)...>::type(xs...));
/// 
/// Requirements
/// ------------
/// 
/// MetafunctionClass must be a:
/// 
/// * [MetafunctionClass](concepts.md#metafunctionclass)
/// 
/// MetafunctionTemplate<Ts...> must be a:
/// 
/// * [Metafunction](concepts.md#metafunction)
/// 
/// T, Template<Ts..>, MetafunctionClass::apply<Ts...>, and
/// MetafunctionTemplate<Ts...>::type must be:
/// 
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     auto v = fit::construct<std::vector<int>>()(5, 5);
///     assert(v.size() == 5);
/// 

#include <fit/detail/forward.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/join.hpp>

#include <initializer_list>

namespace fit { 

template<class Projection, class F>
struct by_adaptor;

namespace detail {

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

    template<class F>
    constexpr by_adaptor<F, construct_f> by(F f) const
    {
        return by_adaptor<F, construct_f>(fit::move(f), *this);
    }
};

template<template<class...> class Template>
struct construct_template_f
{
    constexpr construct_template_f()
    {}
    template<class... Ts, class Result=FIT_JOIN(Template, Ts...), 
        FIT_ENABLE_IF_CONSTRUCTIBLE(Result, Ts...)>
    constexpr Result operator()(Ts&&... xs) const
    {
        return Result(fit::forward<Ts>(xs)...);
    }

    template<class F>
    constexpr by_adaptor<F, construct_template_f> by(F f) const
    {
        return by_adaptor<F, construct_template_f>(fit::move(f), *this);
    }
};

template<class MetafunctionClass>
struct construct_meta_f
{
    constexpr construct_meta_f()
    {}

    template<class... Ts>
    struct apply
    : MetafunctionClass::template apply<Ts...>
    {};

    template<class... Ts, 
        class Metafunction=FIT_JOIN(apply, Ts...), 
        class Result=typename Metafunction::type, 
        FIT_ENABLE_IF_CONSTRUCTIBLE(Result, Ts...)>
    constexpr Result operator()(Ts&&... xs) const
    {
        return Result(fit::forward<Ts>(xs)...);
    }

    template<class F>
    constexpr by_adaptor<F, construct_meta_f> by(F f) const
    {
        return by_adaptor<F, construct_meta_f>(fit::move(f), *this);
    }
};

template<template<class...> class MetafunctionTemplate>
struct construct_meta_template_f
{
    constexpr construct_meta_template_f()
    {}
    template<class... Ts, 
        class Metafunction=FIT_JOIN(MetafunctionTemplate, Ts...), 
        class Result=typename Metafunction::type, 
        FIT_ENABLE_IF_CONSTRUCTIBLE(Result, Ts...)>
    constexpr Result operator()(Ts&&... xs) const
    {
        return Result(fit::forward<Ts>(xs)...);
    }

    template<class F>
    constexpr by_adaptor<F, construct_meta_template_f> by(F f) const
    {
        return by_adaptor<F, construct_meta_template_f>(fit::move(f), *this);
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

template<class T>
constexpr detail::construct_meta_f<T> construct_meta()
{
    return {};
}

template<template<class...> class Template>
constexpr detail::construct_meta_template_f<Template> construct_meta()
{
    return {};
}

} // namespace fit

#endif
