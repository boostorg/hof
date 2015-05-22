/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    static_const.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_STATIC_CONST_H
#define FIT_GUARD_STATIC_CONST_H

namespace fit { namespace detail {

template<class T>
struct static_const_storage
{
    static constexpr T value = T();
};

template<class T>
constexpr T static_const_storage<T>::value;

}

template<class T>
constexpr const T& static_const()
{
    return detail::static_const_storage<T>::value;
}

}

#endif
