/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    inline_var.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_INLINE_VAR_HPP
#define FIT_GUARD_INLINE_VAR_HPP

#include <fit/detail/static_constexpr.hpp>

#if defined(__clang__) || defined(__GNUC__)

#define FIT_INLINE_VAR(var) extern __attribute__((weak)) auto var
#define FIT_INLINE_CONST_VAR(var) extern __attribute__((weak)) constexpr auto var

#define FIT_HAS_INLINE_VAR 1

#elif defined(_WIN32)

#define FIT_INLINE_VAR(var) extern __declspec(selectany) static auto var
#define FIT_INLINE_CONST_VAR(var) extern __declspec(selectany) constexpr auto var
#define FIT_HAS_INLINE_VAR 1

#else
#define FIT_INLINE_VAR(var) static auto var
#define FIT_INLINE_CONST_VAR(var) FIT_STATIC_CONSTEXPR auto var
#define FIT_HAS_INLINE_VAR 0

#endif

#endif
