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
/// the certain parameters. It uses the same signature as `result_of`.
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class Sig>
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
///     static_assert(is_callable<is_callable_class(int)>(), "Not callable");
/// 

#include <utility>
#include <type_traits>

namespace fit {

namespace detail {

template<class T>
struct holder
{
    typedef void type;
};
}

template<class F, class=void>
struct is_callable
: std::false_type
{};

template<class F, class... Args>
struct is_callable<F(Args...), typename detail::holder<
    decltype( std::declval<F>()(std::declval<Args>()...) )
>::type>
: std::true_type
{};

}

#endif
