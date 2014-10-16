/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    delgate.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_DELGATE_H
#define FIT_GUARD_FUNCTION_DELGATE_H

#include <type_traits>
#include <utility>

#ifndef FIT_NO_TYPE_PACK_EXPANSION_IN_TEMPLATE
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define FIT_NO_TYPE_PACK_EXPANSION_IN_TEMPLATE 1
#else
#define FIT_NO_TYPE_PACK_EXPANSION_IN_TEMPLATE 0
#endif
#endif

#define FIT_ENABLE_IF_CONVERTIBLE(...) \
    typename std::enable_if<std::is_convertible<__VA_ARGS__>::value, int>::type = 0

#define FIT_ENABLE_IF_CONVERTIBLE_UNPACK(...) \
    typename std::enable_if<fit::detail::and_<std::is_convertible<__VA_ARGS__>...>::value, int>::type = 0

#define FIT_ENABLE_IF_CONSTRUCTIBLE(...) \
    typename std::enable_if<std::is_constructible<__VA_ARGS__>::value, int>::type = 0

#if FIT_NO_TYPE_PACK_EXPANSION_IN_TEMPLATE

#define FIT_DELGATE_CONSTRUCTOR(C, T, var) \
    template<class... FitXs, typename fit::detail::enable_if_constructible<T, FitXs...>::type = 0> \
    constexpr C(FitXs&&... fit_xs) : var(std::forward<FitXs>(fit_xs)...) {}
    
#else
#define FIT_DELGATE_CONSTRUCTOR(C, T, var) \
    template<class... FitXs, FIT_ENABLE_IF_CONSTRUCTIBLE(T, FitXs&&...)> \
    constexpr C(FitXs&&... fit_xs) : var(std::forward<FitXs>(fit_xs)...) {}

#endif

// TODO: For compilers that support inheriting constructors replace with
// `using Base::Base;`
#define FIT_INHERIT_CONSTRUCTOR(Derived, Base) FIT_DELGATE_CONSTRUCTOR(Derived, Base, Base)

namespace fit {
namespace detail {

template<class X, class... Xs>
struct enable_if_constructible
: std::enable_if<std::is_constructible<X, Xs&&...>::value, int>
{};

template<bool...> struct bool_seq {};
template<class... Ts>
struct and_
: std::is_same<bool_seq<Ts::value...>, bool_seq<(Ts::value, true)...>>::type
{};

}
}

#endif