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
#include <fit/always.hpp>
#include <fit/alias.hpp>

#ifndef FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND
#ifdef __clang__
#define FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND 0
#else
#define FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND 1
#endif
#endif

namespace fit { namespace detail {

template<int I, class T, class U>
struct pair_tag
{};

#if FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND
template<class T, class U>
struct is_related
: std::integral_constant<bool, std::is_base_of<T, U>::value || std::is_base_of<U, T>::value>
{};
template<int I, class T, class U>
struct pair_holder_related
: std::conditional<
        std::is_empty<T>::value && 
        std::is_literal_type<T>::value && 
        is_default_constructible<T>::value, 
    alias_static<T, pair_tag<I, T, U>>,
    alias<T, pair_tag<I, T, U>>
>
{};

template<int I, class T, class U>
struct pair_holder_non_related
: std::conditional<std::is_class<T>::value, 
    alias_inherit<T, pair_tag<I, T, U>>, 
    alias<T, pair_tag<I, T, U>>
>
{};

template<int I, class T, class U>
struct pair_holder
: std::conditional<is_related<T, U>::value, 
    pair_holder_related<I, T, U>,
    pair_holder_non_related<I, T, U>
>::type
{};
#else
template<int I, class T, class U>
struct pair_holder
: std::conditional<std::is_class<T>::value, 
    alias_inherit<T, pair_tag<I, T, U>>, 
    alias<T, pair_tag<I, T, U>>
>
{};
#endif

// TODO: Empty optimizations for MSVC
template<
    class First, 
    class Second, 
    class FirstBase=typename pair_holder<0, First, Second>::type, 
    class SecondBase=typename pair_holder<1, Second, First>::type,
    class=void
>
struct compressed_pair 
: FirstBase, SecondBase
{
    template<class X, class Y, 
        FIT_ENABLE_IF_CONSTRUCTIBLE(First, X&&), 
        FIT_ENABLE_IF_CONSTRUCTIBLE(Second, Y&&)
    >
    constexpr compressed_pair(X&& x, Y&& y) 
    : FirstBase(fit::forward<X>(x)), SecondBase(fit::forward<Y>(y))
    {}

    FIT_INHERIT_DEFAULT(compressed_pair, FirstBase, SecondBase)

    template<class Base, class... Xs>
    constexpr const Base& get_base(Xs&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Xs>
    constexpr const First& first(Xs&&... xs) const
    {
        return alias_value(this->get_base<FirstBase>(xs...), xs...);
    }

    template<class... Xs>
    constexpr const Second& second(Xs&&... xs) const
    {
        return alias_value(this->get_base<SecondBase>(xs...), xs...);
    }

};


}} // namespace fit

#endif
