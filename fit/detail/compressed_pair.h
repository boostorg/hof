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

namespace fit { namespace detail {

template<int N, class T, class>
struct pair_holder : private T
{
    template<class... Ts>
    constexpr const T& get_value(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_INHERIT_CONSTRUCTOR(pair_holder, T)
};

template<class T, class... Ts>
struct always_first
{
    typedef T type;
};

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
    constexpr const Base& get_base(Xs&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Xs>
    constexpr const First& first(Xs&&... xs) const
    {
        return this->get_base<FirstBase>(xs...).get_value(xs...);
    }

    template<class... Xs>
    constexpr const Second& second(Xs&&... xs) const
    {
        return this->get_base<SecondBase>(xs...).get_value(xs...);
    }

};
}}

#endif