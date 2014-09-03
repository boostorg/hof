/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    remove_rvalue_reference.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_REMOVE_RVALUE_REFERENCE_H
#define FIT_GUARD_FUNCTION_REMOVE_RVALUE_REFERENCE_H

namespace fit { namespace detail {

template<class T>
struct remove_rvalue_reference
{
    typedef T type;
};

template<class T>
struct remove_rvalue_reference<T&&>
{
    typedef T type;
};

}}

#endif