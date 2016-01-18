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
/// The `is_callable` metafunction checks if the function is callable with
/// certain parameters.
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [Callable](concepts.md#callable)
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


#include <fit/detail/can_be_called.hpp>
#include <fit/apply.hpp>

namespace fit {

template<class F, class... Ts>
struct is_callable
: detail::can_be_called<detail::apply_f, F, Ts...>
{};

} // namespace fit

#endif
