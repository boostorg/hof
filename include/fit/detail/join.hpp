/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    join.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_DETAIL_JOIN_H
#define FIT_GUARD_FUNCTION_DETAIL_JOIN_H

#include <fit/detail/holder.hpp>

namespace fit { namespace detail {

template<class... Ts>
struct join_args
{};

template<template <class...> class T, class Args, class=void>
struct join_impl
{};

template<template <class...> class T, class... Args>
struct join_impl<T, join_args<Args...>, typename holder<
    T<Args...>
>::type>
{ typedef T<Args...> type; };

template<template <class...> class T, class... Args>
struct join
: join_impl<T, join_args<Args...>>
{};

}} // namespace fit

#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define FIT_JOIN(c, ...) typename fit::detail::join<c, __VA_ARGS__>::type
#else
#define FIT_JOIN(c, ...) c<__VA_ARGS__>

#endif

#endif
