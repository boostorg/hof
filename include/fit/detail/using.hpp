/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    using.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_USING_HPP
#define FIT_GUARD_USING_HPP

#include <fit/config.hpp>

#if FIT_HAS_TEMPLATE_ALIAS
#define FIT_USING(name, ...) using name = __VA_ARGS__
#define FIT_USING_TYPENAME(name, ...) using name = typename __VA_ARGS__
#else
#define FIT_USING(name, ...) struct name : __VA_ARGS__ {}
#define FIT_USING_TYPENAME(name, ...) struct name : __VA_ARGS__ {}
#endif

#endif
