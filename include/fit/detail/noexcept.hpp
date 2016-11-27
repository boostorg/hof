/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    noexcept.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_DETAIL_NOEXCEPT_HPP
#define FIT_GUARD_DETAIL_NOEXCEPT_HPP

#include <fit/config.hpp>

#if FIT_HAS_NOEXCEPT_DEDUCTION
#define FIT_NOEXCEPT(...) noexcept(__VA_ARGS__)
#else
#define FIT_NOEXCEPT(...)
#endif

#endif
