/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    forward.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FORWARD_H
#define BOOST_FIT_GUARD_FORWARD_H

#include <utility>

namespace boost { namespace fit {

// contexpr-friendly forwarding

template<typename T>
constexpr T&& forward(typename std::remove_reference<T>::type& t) noexcept
{ return static_cast<T&&>(t); }


template<typename T>
constexpr T&& forward(typename std::remove_reference<T>::type&& t) noexcept
{
  static_assert(!std::is_lvalue_reference<T>::value, "T must not be an lvalue reference type");
  return static_cast<T&&>(t);
}

#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7) || defined(_MSC_VER)
#define BOOST_FIT_FORWARD(...) boost::fit::forward<__VA_ARGS__>
#else
#define BOOST_FIT_FORWARD(...) static_cast<__VA_ARGS__ &&>
#endif

}} // namespace boost::fit

#endif
