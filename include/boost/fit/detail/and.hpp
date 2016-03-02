/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    and.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_AND_H
#define BOOST_FIT_GUARD_AND_H

#include <type_traits>
#include <boost/fit/detail/using.hpp>
#include <boost/fit/detail/intrinsics.hpp>

namespace boost { namespace fit { namespace detail {

constexpr bool and_c()
{
    return true;
}

template<class... Ts>
constexpr bool and_c(bool b, Ts... bs)
{
    return b && and_c(bs...);
}

#ifdef _MSC_VER

template<class... Ts>
struct and_;

template<class T, class... Ts>
struct and_<T, Ts...>
: std::integral_constant<bool, (T::value && and_<Ts...>::value)>
{};

template<>
struct and_<>
: std::true_type
{};

#define BOOST_FIT_AND_UNPACK(Bs) boost::fit::detail::and_c(Bs...)
#else
template<bool...> struct bool_seq {};
template<class... Ts>
BOOST_FIT_USING(and_, std::is_same<bool_seq<Ts::value...>, bool_seq<(Ts::value, true)...>>);

#define BOOST_FIT_AND_UNPACK(Bs) BOOST_FIT_IS_BASE_OF(boost::fit::detail::bool_seq<Bs...>, boost::fit::detail::bool_seq<(Bs || true)...>)

#endif

}}} // namespace boost::fit

#endif
