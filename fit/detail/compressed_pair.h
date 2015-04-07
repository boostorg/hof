/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    compressed_pair.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_COMPRESSED_PAIR_H
#define FIT_GUARD_COMPRESSED_PAIR_H

#include <fit/detail/delegate.h>
#include <fit/always.h>

#ifndef FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND
#ifdef __clang__
#define FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND 0
#else
#define FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND 1
#endif
#endif

namespace fit { namespace detail {

template<int, class T, class>
struct pair_holder : T
{
    FIT_INHERIT_CONSTRUCTOR(pair_holder, T)

    template<class... Ts>
    constexpr const T& get_value(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }
};

namespace pair_detail {
template<
    class First, 
    class Second, 
    class FirstBase=pair_holder<0, First, Second>, 
    class SecondBase=pair_holder<1, Second, First>
>
struct compressed_pair 
: private FirstBase, private SecondBase
{
    template<class X, class Y, 
        FIT_ENABLE_IF_CONSTRUCTIBLE(X, First), 
        FIT_ENABLE_IF_CONSTRUCTIBLE(Y, Second)
    >
    constexpr compressed_pair(X&& x, Y&& y) 
    : FirstBase(fit::forward<X>(x)), SecondBase(fit::forward<Y>(y))
    {}

    template<class Base, class... Xs>
    constexpr const Base& get_base(Xs&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Xs>
    constexpr const First& first(Xs&&... xs) const
    {
        return this->get_base<FirstBase>(xs...);
    }

    template<class... Xs>
    constexpr const Second& second(Xs&&... xs) const
    {
        return this->get_base<SecondBase>(xs...);
    }

};
#if FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND
template<
    class First, 
    class Second
>
struct non_compressed_pair 
{
    First f;
    Second s;
    template<class X, class Y, 
        FIT_ENABLE_IF_CONSTRUCTIBLE(X, First), 
        FIT_ENABLE_IF_CONSTRUCTIBLE(Y, Second)
    >
    constexpr non_compressed_pair(X&& x, Y&& y) 
    : f(fit::forward<X>(x)), s(fit::forward<Y>(y))
    {}

    template<class... Xs>
    constexpr const First& first(Xs&&...) const
    {
        return this->f;
    }

    template<class... Xs>
    constexpr const Second& second(Xs&&...) const
    {
        return this->s;
    }

};
#endif
}

#if FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND
template<class T, class U>
struct is_related
: std::integral_constant<bool, std::is_base_of<T, U>::value || std::is_base_of<U, T>::value>
{};

template<
    class First, 
    class Second
>
struct compressed_pair_base
: std::conditional<
        is_related<First, Second>::value, 
        pair_detail::non_compressed_pair<First, Second>, 
        pair_detail::compressed_pair<First, Second>
    >
{};

template<
    class First, 
    class Second,
    class Base=typename compressed_pair_base<First, Second>::type
>
struct compressed_pair 
: Base
{
    FIT_INHERIT_CONSTRUCTOR(compressed_pair, Base)
};
#else
using pair_detail::compressed_pair;
#endif

}}

#endif