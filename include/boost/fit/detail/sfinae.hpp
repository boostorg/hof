/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    sfinae.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FUNCTION_DETAIL_SFINAE_H
#define BOOST_FIT_GUARD_FUNCTION_DETAIL_SFINAE_H

#ifndef BOOST_FIT_NO_EXPRESSION_SFINAE
#ifdef _MSC_VER
#define BOOST_FIT_NO_EXPRESSION_SFINAE 1
#else
#define BOOST_FIT_NO_EXPRESSION_SFINAE 0
#endif
#endif

#endif
