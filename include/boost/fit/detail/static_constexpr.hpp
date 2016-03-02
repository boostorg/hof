/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    static_constexpr.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_STATIC_CONSTEXPR_H
#define BOOST_FIT_GUARD_STATIC_CONSTEXPR_H

#ifndef BOOST_FIT_HAS_RELAXED_CONSTEXPR
#ifdef __clang__
#if __has_feature(cxx_relaxed_constexpr)
#define BOOST_FIT_HAS_RELAXED_CONSTEXPR 1
#else
#define BOOST_FIT_HAS_RELAXED_CONSTEXPR 0
#endif
#else
#define BOOST_FIT_HAS_RELAXED_CONSTEXPR 0
#endif
#endif

#if BOOST_FIT_HAS_RELAXED_CONSTEXPR || defined(_MSC_VER)
#define BOOST_FIT_STATIC_CONSTEXPR const constexpr
#else
#define BOOST_FIT_STATIC_CONSTEXPR static constexpr
#endif

#endif
