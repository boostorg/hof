/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    join.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_DETAIL_JOIN_H
#define FIT_GUARD_FUNCTION_DETAIL_JOIN_H

namespace fit { namespace detail {

template<template <typename...> class T, typename... Args>
struct join
{ typedef T<Args...> type; };

}}

#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define FIT_JOIN(c, ...) typename fit::detail::join<c, __VA_ARGS__>::type
#else
#define FIT_JOIN(c, ...) c<__VA_ARGS__>

#endif

#endif
