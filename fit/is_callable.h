/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    is_callable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_IS_CALLABLE_H
#define FIT_GUARD_IS_CALLABLE_H

/// is_callable
/// ===========
/// 
/// Description
/// -----------
/// 
/// The `is_callable` metafunction checks if the function object is callable with
/// the certain parameters.
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class F, class... Ts>
///     struct is_callable;
/// 
/// Example
/// -------
/// 
///     struct is_callable_class
///     {
///         void operator()(int) const
///         {
///         }
///     };
///     static_assert(is_callable<is_callable_class, int>(), "Not callable");
/// 

#include <utility>
#include <type_traits>
#include <fit/detail/holder.h>
#include <fit/detail/and.h>
#include <fit/detail/sfinae.h>

namespace fit {

#if FIT_NO_EXPRESSION_SFINAE
namespace detail {

struct dont_care
{
    dont_care(...);
};

template<class T>
struct never_care
{
    typedef dont_care type;
};

struct cant_be_called_type
{};

template<class F, class... Ts>
struct is_callable_wrapper : std::remove_cv<typename std::remove_reference<F>::type>::type
{
    is_callable_wrapper();
    typedef cant_be_called_type const &(*pointer_to_function)(typename never_care<Ts>::type...);
    operator pointer_to_function() const;
};

template<class T>
struct not_
: std::integral_constant<bool, !T::value>
{};

template<class F, class... Ts>
struct can_be_called
: not_<std::is_same<cant_be_called_type, typename std::decay<decltype(
    is_callable_wrapper<F, Ts...>()(std::declval<Ts>()...)
)>::type>>
{};

template<class F, class... Ts>
struct check_args;

template<class Res, class... Ts, class... Us>
struct check_args<Res(Us...), Ts...>
: and_<std::is_convertible<Ts, Us>...>
{};

template<class Res, class... Ts, class... Us>
struct can_be_called<Res(*)(Us...), Ts...>
: std::conditional<sizeof...(Ts) == sizeof...(Us), 
    check_args<Res(Us...), Ts...>, 
    std::false_type
>::type
{};

template<class Res, class... Ts, class... Us>
struct can_be_called<Res(Us...), Ts...>
: std::conditional<sizeof...(Ts) == sizeof...(Us), 
    check_args<Res(Us...), Ts...>, 
    std::false_type
>::type
{};
}

template<class F, class... Ts>
struct is_callable
: detail::can_be_called<F, Ts...>
{};

#else
namespace detail {
template<class... Ts>
struct callable_args
{};

template<class F, class Args, class=void>
struct is_callable_impl
: std::false_type
{};

template<class F, class... Args>
struct is_callable_impl<F, callable_args<Args...>, typename detail::holder<
    decltype( std::declval<F>()(std::declval<Args>()...) )
>::type>
: std::true_type
{};
}


template<class F, class... Ts>
struct is_callable
: detail::is_callable_impl<F, detail::callable_args<Ts...>>
{};

#endif

}

#endif
