/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    compressed_pair.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_COMPRESSED_PAIR_H
#define FIT_GUARD_COMPRESSED_PAIR_H

#include <fit/detail/delegate.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/forward.hpp>
#include <fit/config.hpp>
#include <fit/always.hpp>
#include <fit/alias.hpp>

#ifndef FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND
#define FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND !FIT_HAS_EBO
#endif

namespace fit { namespace detail {

template<class First, class Second, class=void>
struct compressed_pair;

template<int I, class T, class U>
struct pair_tag
{};

#if FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND

template<class T, class U>
struct is_same_template
: std::false_type
{};

template<template<class...> class X, class... Ts, class... Us>
struct is_same_template<X<Ts...>, X<Us...>>
: std::true_type
{};

template<class T, class U>
struct is_related_template
: is_same_template<T, U>
{};

template<class T, class U>
struct is_related
: std::integral_constant<bool, std::is_base_of<T, U>::value || std::is_base_of<U, T>::value || is_same_template<T, U>::value>
{};

template<int I, class T, class U>
struct pair_holder
: std::conditional<(
    is_related<T, U>::value), 
    detail::alias_empty<T, pair_tag<I, T, U>>,
    detail::alias_try_inherit<T, pair_tag<I, T, U>>
>::type
{};
#else
template<int I, class T, class U>
struct pair_holder
: detail::alias_try_inherit<T, pair_tag<I, T, U>>
{};
#endif

// TODO: Empty optimizations for MSVC
template<
    class First, 
    class Second
>
struct compressed_pair<First, Second>
: pair_holder<0, First, Second>::type, pair_holder<1, Second, First>::type
{
    typedef typename pair_holder<0, First, Second>::type first_base;
    typedef typename pair_holder<1, Second, First>::type second_base;
    template<class X, class Y, 
        FIT_ENABLE_IF_CONSTRUCTIBLE(First, X&&), 
        FIT_ENABLE_IF_CONSTRUCTIBLE(Second, Y&&)
    >
    constexpr compressed_pair(X&& x, Y&& y) 
    noexcept(FIT_IS_NOTHROW_CONSTRUCTIBLE(first_base, X&&) && FIT_IS_NOTHROW_CONSTRUCTIBLE(second_base, Y&&))
    : first_base(FIT_FORWARD(X)(x)), second_base(FIT_FORWARD(Y)(y))
    {}

    FIT_INHERIT_DEFAULT(compressed_pair, first_base, second_base)

    template<class Base, class... Xs>
    constexpr const Base& get_alias_base(Xs&&... xs) const noexcept
    {
#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7)
        return static_cast<const Base&>(*this);
#else
        return always_ref(*this)(xs...);
#endif
    }

    template<class... Xs>
    constexpr const First& first(Xs&&... xs) const noexcept
    {
        return alias_value(this->get_alias_base<first_base>(xs...), xs...);
    }

    template<class... Xs>
    constexpr const Second& second(Xs&&... xs) const noexcept
    {
        return alias_value(this->get_alias_base<second_base>(xs...), xs...);
    }

};

template<class T, class U>
constexpr compressed_pair<T, U> make_compressed_pair(T x, U y)
noexcept(FIT_IS_NOTHROW_MOVE_CONSTRUCTIBLE(T) && FIT_IS_NOTHROW_MOVE_CONSTRUCTIBLE(U))
{
    return {static_cast<T&&>(x), static_cast<U&&>(y)};
}


}} // namespace fit

#endif
