/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    static_constexpr.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_STATIC_CONSTEXPR_H
#define FIT_GUARD_STATIC_CONSTEXPR_H

#include <fit/config.hpp>

#if FIT_HAS_RELAXED_CONSTEXPR || defined(_MSC_VER)
#define FIT_STATIC_CONSTEXPR const constexpr
#else
#define FIT_STATIC_CONSTEXPR static constexpr
#endif

#endif
