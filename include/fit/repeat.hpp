/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    repeat.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_REPEAT_H
#define FIT_GUARD_REPEAT_H

/// repeat
/// ======
/// 
/// Description
/// -----------
/// 
/// The `repeat` function decorator will repeatedly apply a function a given
/// number of times.
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class IntegralConstant>
///     constexpr repeat_adaptor<IntegralConstant> repeat(IntegralConstant);
/// 
/// Requirements
/// ------------
/// 
/// IntegralConstant must be:
/// 
/// * IntegralConstant
/// 
/// Example
/// -------
/// 
///     struct increment
///     {
///         template<class T>
///         constexpr T operator()(T x) const
///         {
///             return x + 1;
///         }
///     };
/// 
///     constexpr auto increment_by_5 = fit::repeat(std::integral_constant<int, 5>())(increment());
///     assert(increment_by_5(1) == 6);
/// 

#include <fit/always.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/result_of.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/sfinae.hpp>
#include <fit/detail/static_const_var.hpp>
#include <fit/decorate.hpp>
#include <fit/conditional.hpp>

#include <stdexcept>

namespace fit { namespace detail {

template<int N>
struct repeater
{
    template<class F, class... Ts>
    constexpr FIT_SFINAE_RESULT(repeater<N-1>, id_<const F&>, result_of<const F&, id_<Ts>...>) 
    operator()(const F& f, Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        repeater<N-1>()(f, f(FIT_FORWARD(Ts)(xs)...))
    );
};

template<>
struct repeater<0>
{
    template<class F, class T>
    constexpr T operator()(const F&, T&& x) const
    {
        return x;
    }
};

struct repeat_constant_decorator
{
    template<class Integral, class F, class... Ts>
    constexpr auto operator()(Integral, const F& f, Ts&&... xs) const FIT_RETURNS
    (
        detail::repeater<Integral::type::value>()
        (
            f, 
            FIT_FORWARD(Ts)(xs)...
        )
    );
};

template<int Limit>
struct repeat_integral_decorator
{
    // template<class Integral, class F, class... Ts>
    // constexpr auto operator()(Integral n, const F& f, Ts&&... xs) const FIT_RETURNS
    // (
    //     (n) ? 
    //         repeat_integral_decorator<!Switch>()(--n, f, f(FIT_FORWARD(Ts)(xs)...)) :
    //         throw std::runtime_error("")
    // );

    template<class Integral, class F, class T, class... Ts, class Self=repeat_integral_decorator<Limit-1>>
    constexpr auto operator()(Integral n, const F& f, T&& x, Ts&&... xs) const FIT_RETURNS
    (
        (n) ? 
            Self()(--n, f, f(FIT_FORWARD(T)(x), FIT_FORWARD(Ts)(xs)...)) :
            FIT_FORWARD(T)(x)
    );
};

template<>
struct repeat_integral_decorator<0>
{
    template<class Integral, class F, class T, class... Ts, class Self=repeat_integral_decorator<0>>
    T operator()(Integral n, const F& f, T&& x, Ts&&... xs) const
    {
        return (n) ? 
            Self()(--n, f, f(FIT_FORWARD(T)(x), FIT_FORWARD(Ts)(xs)...)) :
            FIT_FORWARD(T)(x);
    }
};

}

FIT_DECLARE_STATIC_VAR(repeat, decorate_adaptor<
    fit::conditional_adaptor<
    detail::repeat_constant_decorator, 
    detail::repeat_integral_decorator<200>
>>);

} // namespace fit

#endif
