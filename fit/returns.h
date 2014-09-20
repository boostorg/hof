/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    returns.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_RETURNS_H
#define FIT_GUARD_RETURNS_H

#ifndef FIT_HAS_COMPLETE_DECLTYPE
#define FIT_HAS_COMPLETE_DECLTYPE 0
#endif

#include <utility>

#if FIT_HAS_COMPLETE_DECLTYPE
#define FIT_RETURNS(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; } static_assert(true, "")
#define FIT_THIS this
#define FIT_RETURNS_CLASS(...)
#else
#include <fit/detail/pp.h>

#define FIT_RETURNS_RETURN(...) return FIT_RETURNS_RETURN_X(FIT_PP_WALL(__VA_ARGS__))
#define FIT_RETURNS_RETURN_X(...) __VA_ARGS__

#define FIT_RETURNS_DECLTYPE(...) decltype(FIT_RETURNS_DECLTYPE_CONTEXT(__VA_ARGS__))

#define FIT_RETURNS_DECLTYPE_CONTEXT(...) FIT_RETURNS_DECLTYPE_CONTEXT_X(FIT_PP_WALL(__VA_ARGS__))
#define FIT_RETURNS_DECLTYPE_CONTEXT_X(...) __VA_ARGS__

#define FIT_RETURNS_THIS() FIT_PP_IIF(FIT_PP_IS_PAREN(FIT_RETURNS_DECLTYPE_CONTEXT(())))(this, std::declval<fit_this_type>())

#define FIT_THIS FIT_PP_RAIL(FIT_RETURNS_THIS)()

#define FIT_RETURNS_CLASS(...) typedef __VA_ARGS__* fit_this_type

#define FIT_RETURNS(...) -> FIT_RETURNS_DECLTYPE(__VA_ARGS__) { FIT_RETURNS_RETURN(__VA_ARGS__); } static_assert(true, "")

#endif

#define FIT_AUTO_FORWARD(x) std::forward<decltype(x)>(x)

#endif