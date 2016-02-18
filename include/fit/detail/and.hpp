/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    and.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_AND_H
#define FIT_GUARD_AND_H

#include <type_traits>
#include <fit/detail/using.hpp>
#include <fit/detail/intrinsics.hpp>

namespace fit { namespace detail {

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
#define FIT_AND_UNPACK(Bs) and_<std::integral_constant<bool, Bs>...>::value
#else
template<bool...> struct bool_seq {};
template<class... Ts>
FIT_USING(and_, std::is_same<bool_seq<Ts::value...>, bool_seq<(Ts::value, true)...>>);

template<bool... Bs>
FIT_USING(and_c, std::is_same<bool_seq<Bs...>, bool_seq<(Bs, true)...>>);

#define FIT_AND_UNPACK(Bs) FIT_IS_BASE_OF(fit::detail::bool_seq<Bs...>, fit::detail::bool_seq<(Bs || true)...>)
#endif

}} // namespace fit

#endif
