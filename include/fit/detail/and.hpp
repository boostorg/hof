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

#define FIT_AND_UNPACK(Bs) (fit::detail::and_c(Bs...))
#else
template<bool...> struct bool_seq {};
template<class... Ts>
FIT_USING(and_, std::is_same<bool_seq<Ts::value...>, bool_seq<(Ts::value, true)...>>);

#define FIT_AND_UNPACK(Bs) FIT_IS_BASE_OF(fit::detail::bool_seq<Bs...>, fit::detail::bool_seq<(Bs || true)...>)

#endif

}} // namespace fit

#endif
