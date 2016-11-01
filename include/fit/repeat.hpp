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
///     template<class Integral>
///     constexpr repeat_adaptor<Integral> repeat(Integral);
/// 
/// Requirements
/// ------------
/// 
/// Integral must be:
/// 
/// * Integral
/// 
/// Or:
/// 
/// * IntegralConstant
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
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
///     int main() {
///         auto increment_by_5 = fit::repeat(std::integral_constant<int, 5>())(increment());
///         assert(increment_by_5(1) == 6);
///     }
/// 

#include <fit/always.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/result_of.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/static_const_var.hpp>
#include <fit/decorate.hpp>
#include <fit/conditional.hpp>
#include <fit/detail/recursive_constexpr_depth.hpp>

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

template<int Depth>
struct repeat_integral_decorator
{
    template<class Integral, class F, class T, class... Ts, class Self=repeat_integral_decorator<Depth-1>>
    constexpr auto operator()(Integral n, const F& f, T&& x, Ts&&... xs) const FIT_RETURNS
    (
        (n) ? 
            Self()(n-1, f, f(FIT_FORWARD(T)(x), FIT_FORWARD(Ts)(xs)...)) :
            FIT_FORWARD(T)(x)
    );
};

template<>
struct repeat_integral_decorator<0>
{
    template<class Integral, class F, class T, class Self=repeat_integral_decorator<0>>
#if FIT_HAS_RELAXED_CONSTEXPR
    constexpr
#endif
    auto operator()(Integral n, const F& f, T x) const -> decltype(f(FIT_FORWARD(T)(x)))
    {
        while(n > 0)
        {
            n--;
            x = f(FIT_FORWARD(T)(x));
        }
        return x;
    }
    // TODO: Add overload for lvalue
};

}

FIT_DECLARE_STATIC_VAR(repeat, decorate_adaptor<
    fit::conditional_adaptor<
    detail::repeat_constant_decorator, 
#if FIT_HAS_RELAXED_CONSTEXPR
    detail::repeat_integral_decorator<1>
#else
    detail::repeat_integral_decorator<FIT_RECURSIVE_CONSTEXPR_DEPTH>
#endif
>>);

} // namespace fit

#endif
