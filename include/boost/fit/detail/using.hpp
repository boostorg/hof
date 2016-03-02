/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    using.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_USING_HPP
#define BOOST_FIT_GUARD_USING_HPP

#ifndef BOOST_FIT_HAS_TEMPLATE_ALIAS
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8
#define BOOST_FIT_HAS_TEMPLATE_ALIAS 0
#else
#define BOOST_FIT_HAS_TEMPLATE_ALIAS 1
#endif
#endif

#if BOOST_FIT_HAS_TEMPLATE_ALIAS
#define BOOST_FIT_USING(name, ...) using name = __VA_ARGS__
#define BOOST_FIT_USING_TYPENAME(name, ...) using name = typename __VA_ARGS__
#else
#define BOOST_FIT_USING(name, ...) struct name : __VA_ARGS__ {}
#define BOOST_FIT_USING_TYPENAME(name, ...) struct name : __VA_ARGS__ {}
#endif

#endif
