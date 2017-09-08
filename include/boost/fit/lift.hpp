/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    lift.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FUNCTION_LIFT_H
#define BOOST_FIT_GUARD_FUNCTION_LIFT_H

/// BOOST_FIT_LIFT
/// ========
/// 
/// Description
/// -----------
/// 
/// The macros `BOOST_FIT_LIFT` and `BOOST_FIT_LIFT_CLASS` provide a lift operator that
/// will wrap a template function in a function object so it can be passed to
/// higher-order functions. The `BOOST_FIT_LIFT` macro will wrap the function using
/// a generic lambda. As such, it will not preserve `constexpr`. The
/// `BOOST_FIT_LIFT_CLASS` can be used to declare a class that will wrap function.
/// This will preserve `constexpr` and it can be used on older compilers that
/// don't support generic lambdas yet.
/// 
/// Synopsis
/// --------
/// 
///     // Wrap the function in a generic lambda
///     #define BOOST_FIT_LIFT(...)
/// 
///     // Declare a class named `name` that will forward to the function
///     #define BOOST_FIT_LIFT_CLASS(name, ...)
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
///     #include <algorithm>
/// 
///     // Declare the class `max_f`
///     BOOST_FIT_LIFT_CLASS(max_f, std::max);
/// 
///     int main() {
///         auto my_max = BOOST_FIT_LIFT(std::max);
///         assert(my_max(3, 4) == std::max(3, 4));
///         assert(max_f()(3, 4) == std::max(3, 4));
///     }
/// 

#include <boost/fit/detail/delegate.hpp>
#include <boost/fit/returns.hpp>
#include <boost/fit/lambda.hpp>
#include <boost/fit/detail/forward.hpp>

namespace boost { namespace fit { namespace detail {

template<class F, class NoExcept>
struct lift_noexcept : F
{
    BOOST_FIT_INHERIT_CONSTRUCTOR(lift_noexcept, F);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const
    noexcept(decltype(std::declval<NoExcept>()(BOOST_FIT_FORWARD(Ts)(xs)...)){})
    -> decltype(std::declval<F>()(BOOST_FIT_FORWARD(Ts)(xs)...))
    { return F(*this)(BOOST_FIT_FORWARD(Ts)(xs)...);}
};

template<class F, class NoExcept>
constexpr lift_noexcept<F, NoExcept> make_lift_noexcept(F f, NoExcept)
{
    return {f};
}

}

}} // namespace boost::fit

#define BOOST_FIT_LIFT_IS_NOEXCEPT(...) std::integral_constant<bool, noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))>{}

#if defined (_MSC_VER)
#define BOOST_FIT_LIFT(...) (BOOST_FIT_STATIC_LAMBDA { BOOST_FIT_LIFT_CLASS(fit_local_lift_t, __VA_ARGS__); return fit_local_lift_t(); }())
#elif defined (__clang__)
#define BOOST_FIT_LIFT(...) (boost::fit::detail::make_lift_noexcept( \
    BOOST_FIT_STATIC_LAMBDA(auto&&... xs) \
    -> decltype((__VA_ARGS__)(BOOST_FIT_FORWARD(decltype(xs))(xs)...)) \
    { return (__VA_ARGS__)(BOOST_FIT_FORWARD(decltype(xs))(xs)...); }, \
    BOOST_FIT_STATIC_LAMBDA(auto&&... xs) { return BOOST_FIT_LIFT_IS_NOEXCEPT((__VA_ARGS__)(BOOST_FIT_FORWARD(decltype(xs))(xs)...)); } \
))
#else
#define BOOST_FIT_LIFT(...) (BOOST_FIT_STATIC_LAMBDA(auto&&... xs) BOOST_FIT_RETURNS((__VA_ARGS__)(BOOST_FIT_FORWARD(decltype(xs))(xs)...)))
#endif

#define BOOST_FIT_LIFT_CLASS(name, ...) \
struct name \
{ \
    template<class... Ts> \
    constexpr auto operator()(Ts&&... xs) const \
    BOOST_FIT_RETURNS((__VA_ARGS__)(BOOST_FIT_FORWARD(Ts)(xs)...)) \
}

#endif
