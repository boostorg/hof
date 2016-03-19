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
struct pair_holder
: std::conditional<is_related<T, U>::value, 
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
    : FirstBase(FIT_FORWARD(X)(x)), SecondBase(FIT_FORWARD(Y)(y))
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

template<class T, class U>
constexpr compressed_pair<T, U> make_compressed_pair(T x, U y)
{
    return {static_cast<T&&>(x), static_cast<U&&>(y)};
}


}} // namespace fit

#endif
