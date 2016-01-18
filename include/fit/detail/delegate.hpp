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
#include <fit/detail/and.hpp>
#include <fit/detail/holder.hpp>
#include <fit/detail/forward.hpp>

#ifndef FIT_NO_TYPE_PACK_EXPANSION_IN_TEMPLATE
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define FIT_NO_TYPE_PACK_EXPANSION_IN_TEMPLATE 1
#else
#define FIT_NO_TYPE_PACK_EXPANSION_IN_TEMPLATE 0
#endif
#endif

#ifndef FIT_NO_STD_DEFAULT_CONSTRUCTIBLE
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define FIT_NO_STD_DEFAULT_CONSTRUCTIBLE 1
#else
#define FIT_NO_STD_DEFAULT_CONSTRUCTIBLE 0
#endif
#endif

#define FIT_ENABLE_IF_CONVERTIBLE(...) \
    class=typename std::enable_if<std::is_convertible<__VA_ARGS__>::value>::type

#define FIT_ENABLE_IF_CONVERTIBLE_UNPACK(...) \
    class=typename std::enable_if<fit::detail::and_<std::is_convertible<__VA_ARGS__>...>::value>::type

#define FIT_ENABLE_IF_CONSTRUCTIBLE(...) \
    class=typename std::enable_if<std::is_constructible<__VA_ARGS__>::value>::type

#define FIT_INHERIT_DEFAULT(C, ...) \
    template<bool FitPrivateEnableBool_##__LINE__=true, \
    class=typename std::enable_if<FitPrivateEnableBool_##__LINE__ && fit::detail::is_default_constructible<__VA_ARGS__>::value>::type> \
    constexpr C() {}

#define FIT_INHERIT_DEFAULT_EMPTY(C, ...) \
    template<bool FitPrivateEnableBool_##__LINE__=true, \
    class=typename std::enable_if<FitPrivateEnableBool_##__LINE__ && \
        fit::detail::is_default_constructible<__VA_ARGS__>::value && std::is_empty<__VA_ARGS__>::value \
    >::type> \
    constexpr C() {}

#if FIT_NO_TYPE_PACK_EXPANSION_IN_TEMPLATE

#define FIT_DELGATE_CONSTRUCTOR(C, T, var) \
    template<class... FitXs, typename fit::detail::enable_if_constructible<C, T, FitXs...>::type = 0> \
    constexpr C(FitXs&&... fit_xs) : var((FitXs&&)fit::forward<FitXs>(fit_xs)...) {}
    
#else
#define FIT_DELGATE_CONSTRUCTOR(C, T, var) \
    template<class... FitXs, FIT_ENABLE_IF_CONSTRUCTIBLE(T, FitXs&&...)> \
    constexpr C(FitXs&&... fit_xs) : var(fit::forward<FitXs>(fit_xs)...) {}

#endif

// TODO: For compilers that support inheriting constructors replace with
// `using Base::Base;`
#define FIT_INHERIT_CONSTRUCTOR(Derived, Base) FIT_DELGATE_CONSTRUCTOR(Derived, Base, Base)

namespace fit {
namespace detail {


template<class T, class=void>
struct is_default_constructible_check
: std::false_type
{};

template<class T>
struct is_default_constructible_check<T, typename holder<
    decltype(T())
>::type>
: std::true_type
{};

template<class T>
struct is_default_constructible_helper
: std::conditional<(std::is_reference<T>::value), 
    std::false_type,
    is_default_constructible_check<T>
>::type
{};

template<class... Xs>
struct is_default_constructible
#if FIT_NO_STD_DEFAULT_CONSTRUCTIBLE
: and_<is_default_constructible_helper<Xs>...>
#else
: and_<std::is_default_constructible<Xs>...>
#endif
{};

template<class T, class... Xs>
struct is_constructible
: std::is_constructible<T, Xs...>
{};

template<class T>
struct is_constructible<T>
: is_default_constructible<T>
{};

template<class C, class X, class... Xs>
struct enable_if_constructible
: std::enable_if<is_constructible<X, Xs&&...>::value, int>
{};

}
} // namespace fit

#endif
