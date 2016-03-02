/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    compressed_pair.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_COMPRESSED_PAIR_H
#define BOOST_FIT_GUARD_COMPRESSED_PAIR_H

#include <boost/fit/detail/delegate.hpp>
#include <boost/fit/detail/move.hpp>
#include <boost/fit/detail/forward.hpp>
#include <boost/fit/always.hpp>
#include <boost/fit/alias.hpp>

#ifndef BOOST_FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND
#ifdef __clang__
#define BOOST_FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND 0
#else
#define BOOST_FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND 1
#endif
#endif

namespace boost { namespace fit { namespace detail {

template<int I, class T, class U>
struct pair_tag
{};

#if BOOST_FIT_COMPRESSED_PAIR_USE_EBO_WORKAROUND
template<class T, class U>
struct is_related
: std::integral_constant<bool, std::is_base_of<T, U>::value || std::is_base_of<U, T>::value>
{};
template<int I, class T, class U>
struct pair_holder_related
: std::conditional<
        BOOST_FIT_IS_EMPTY(T) && 
        BOOST_FIT_IS_LITERAL(T) && 
        BOOST_FIT_IS_DEFAULT_CONSTRUCTIBLE(T), 
    alias_static<T, pair_tag<I, T, U>>,
    alias<T, pair_tag<I, T, U>>
>
{};

template<int I, class T, class U>
struct pair_holder_non_related
: std::conditional<BOOST_FIT_IS_CLASS(T), 
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
: std::conditional<BOOST_FIT_IS_CLASS(T), 
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
        BOOST_FIT_ENABLE_IF_CONSTRUCTIBLE(First, X&&), 
        BOOST_FIT_ENABLE_IF_CONSTRUCTIBLE(Second, Y&&)
    >
    constexpr compressed_pair(X&& x, Y&& y) 
    : FirstBase(BOOST_FIT_FORWARD(X)(x)), SecondBase(BOOST_FIT_FORWARD(Y)(y))
    {}

    BOOST_FIT_INHERIT_DEFAULT(compressed_pair, FirstBase, SecondBase)

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


}}} // namespace boost::fit

#endif
