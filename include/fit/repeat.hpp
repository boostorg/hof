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

namespace fit { namespace detail {

template<int N>
struct repeater
{
    template<class F, class... Ts>
    constexpr FIT_SFINAE_RESULT(repeater<N-1>, id_<const F&>, result_of<const F&, id_<Ts>...>) 
    operator()(const F& f, Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        repeater<N-1>()(f, f(fit::forward<Ts>(xs)...))
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

struct repeat_decorator
{
    template<class T, class F, class... Ts>
    constexpr auto operator()(T, const F& f, Ts&&... xs) const FIT_RETURNS
    (
        detail::repeater<T::value>()
        (
            f, 
            fit::forward<Ts>(xs)...
        )
    );
};

}

FIT_DECLARE_STATIC_VAR(repeat, decorate_adaptor<detail::repeat_decorator>);

} // namespace fit

#endif
