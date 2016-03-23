/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    lift.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_LIFT_H
#define FIT_GUARD_FUNCTION_LIFT_H

/// FIT_LIFT
/// ========
/// 
/// Description
/// -----------
/// 
/// The macros `FIT_LIFT` and `FIT_LIFT_CLASS` provide a lift operator that
/// will wrap a template function in a function object so it can be passed to
/// higher-order functions. The `FIT_LIFT` macro will wrap the function using
/// a generic lambda. As such, it will not preserve `constexpr`. The
/// `FIT_LIFT_CLASS` can be used to declare a class that will wrap function.
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
///     #include <fit.hpp>
///     #include <cassert>
/// 
///     // Declare the class `max_f`
///     FIT_LIFT_CLASS(max_f, std::max);
///     int main() {
///         auto my_max = FIT_LIFT(std::max);
///         assert(my_max(3, 4) == std::max(3, 4));
///         assert(max_f()(3, 4) == std::max(3, 4));
///     }
/// 

#include <fit/returns.hpp>
#include <fit/lambda.hpp>
#include <fit/detail/forward.hpp>

#ifdef _MSC_VER
#define FIT_LIFT(...) (FIT_STATIC_LAMBDA { FIT_LIFT_CLASS(fit_local_lift_t, __VA_ARGS__); return fit_local_lift_t(); }())
#else
#define FIT_LIFT(...) (FIT_STATIC_LAMBDA(auto&&... xs) FIT_RETURNS(__VA_ARGS__(FIT_FORWARD(decltype(xs))(xs)...)))
#endif

#define FIT_LIFT_CLASS(name, ...) \
struct name \
{ \
    template<class... Ts> \
    constexpr auto operator()(Ts&&... xs) const \
    FIT_RETURNS(__VA_ARGS__(FIT_FORWARD(Ts)(xs)...)) \
}

#endif
