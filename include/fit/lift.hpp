/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    lift.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_LIFT_H
#define FIT_GUARD_FUNCTION_LIFT_H

/// lift
/// ====
/// 
/// Description
/// -----------
/// 
/// The `lift` operator will wrap a template function in a function object so it
/// can be passed to higher-order functions. The `FIT_LIFT` macro will wrap the
/// function using a generic lambda. As such, it will not preserve `constexpr`.
/// The `FIT_LIFT_CLASS` can be used to declare a class that will wrap function.
/// This will preserve `constexpr` and it can be used on older compilers that
/// don't support generic lambdas yet.
/// 
/// Synopsis
/// --------
/// 
///     // Wrap the function in a generic lambda
///     #define FIT_LIFT(...)
/// 
///     // Declare a class named `name` that will forward to the function
///     #define FIT_LIFT_CLASS(name, ...)
/// 
/// Example
/// -------
/// 
///     auto my_max = FIT_LIFT(std::max);
///     assert(my_max(3, 4) == std::max(3, 4));
///     // Declare the class `max_f`
///     FIT_LIFT_CLASS(max_f, std::max);
///     assert(max_f()(3, 4) == std::max(3, 4));
/// 

#include <fit/returns.hpp>
#include <fit/detail/forward.hpp>

#define FIT_LIFT(...) [](auto&&... xs) FIT_RETURNS(__VA_ARGS__(fit::forward<decltype(xs)>(xs)...))

#define FIT_LIFT_CLASS(name, ...) \
struct name \
{ \
    template<class... Ts> \
    constexpr auto operator()(Ts&&... xs) const \
    FIT_RETURNS(__VA_ARGS__(fit::forward<Ts>(xs)...)) \
}

#endif
