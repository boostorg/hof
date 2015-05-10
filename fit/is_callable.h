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

namespace fit {

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

// Deprecated but backwards compatible version
template<class F, class... Ts>
struct is_callable<F(Ts...)>
: detail::is_callable_impl<F, detail::callable_args<Ts...>>
{};

}

#endif
