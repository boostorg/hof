/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    repeat_while.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_REPEAT_WHILE_H
#define FIT_GUARD_REPEAT_WHILE_H

/// repeat_while
/// ======
/// 
/// Description
/// -----------
/// 
/// The `repeat_while` function decorator will repeatedly apply a function while
/// the predicate returns a boolean that is true. If the predicate returns an
/// `IntergralConstant` then the predicate is only evaluated at compile-time.
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class Predicate>
///     constexpr auto repeat_while(Predicate predicate);
/// 
/// Requirements
/// ------------
/// 
/// Predicate must be:
/// 
/// * [ConstFunctionObject](concepts.md#constfunctionobject)
/// * MoveConstructible
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
///         constexpr std::integral_constant<int, T::value + 1> operator()(T) const
///         {
///             return std::integral_constant<int, T::value + 1>();
///         }
///     };
/// 
///     struct not_6
///     {
///         template<class T>
///         constexpr std::integral_constant<bool, (T::value != 6)> 
///         operator()(T) const
///         {
///             return std::integral_constant<bool, (T::value != 6)>();
///         }
///     };
/// 
///     typedef std::integral_constant<int, 1> one;
///     typedef std::integral_constant<int, 6> six;
/// 
///     int main() {
///         auto increment_until_6 = fit::repeat_while(not_6())(increment());
///         static_assert(std::is_same<six, decltype(increment_until_6(one()))>::value, "Error");
///     }
/// 

#include <fit/always.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/result_of.hpp>
#include <fit/detail/move.hpp>
#include <fit/decorate.hpp>
#include <fit/detail/sfinae.hpp>
#include <fit/detail/static_const_var.hpp>
#include <fit/conditional.hpp>
#include <fit/detail/recursive_constexpr_depth.hpp>

namespace fit { namespace detail {

template<class P, class... Ts>
struct compute_predicate
{
    typedef decltype(std::declval<P>()(std::declval<Ts>()...)) type;
};

template<bool B>
struct while_repeater
{
    template<class F, class P, class... Ts>
    constexpr FIT_SFINAE_RESULT(while_repeater<
        compute_predicate<P, typename result_of<const F&, id_<Ts>...>::type>::type::value
    >, id_<const F&>, id_<const P&>, result_of<const F&, id_<Ts>...>) 
    operator()(const F& f, const P& p, Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        while_repeater<
            compute_predicate<P, decltype(f(FIT_FORWARD(Ts)(xs)...))>::type::value
        >()(f, p, f(FIT_FORWARD(Ts)(xs)...))
    );
};

template<>
struct while_repeater<false>
{
    template<class F, class P, class T>
    constexpr T operator()(const F&, const P&, T&& x) const
    {
        return x;
    }
};

struct repeat_while_constant_decorator
{
    template<class P, class F, class... Ts>
    constexpr auto operator()(const P& p, const F& f, Ts&&... xs) const FIT_RETURNS
    (
        detail::while_repeater<
            detail::compute_predicate<P, decltype(std::declval<F>()(FIT_FORWARD(Ts)(xs)...))>::type::value
        >()
        (
            f, 
            p, 
            FIT_FORWARD(Ts)(xs)...
        )
    );
};

template<int Depth>
struct repeat_while_integral_decorator
{
    template<class P, class F, class T, class... Ts, class Self=repeat_while_integral_decorator<Depth-1>>
    constexpr auto operator()(const P& p, const F& f, T&& x, Ts&&... xs) const FIT_RETURNS
    (
        (p(x, FIT_FORWARD(Ts)(xs)...)) ? 
            Self()(
                p, 
                f, 
                f(x, FIT_FORWARD(Ts)(xs)...)
            ) : 
            FIT_FORWARD(T)(x)
    );
};

template<>
struct repeat_while_integral_decorator<0>
{
    template<class P, class F, class T, class Self=repeat_while_integral_decorator<0>>
    auto operator()(const P& p, const F& f, T&& x) const -> decltype(f(x))
    {
        return (p(x)) ? 
            Self()( 
                p, 
                f, 
                f(x)
            ) : 
            FIT_FORWARD(T)(x);
    }
};
}

FIT_DECLARE_STATIC_VAR(repeat_while, decorate_adaptor<
    fit::conditional_adaptor<
        detail::repeat_while_constant_decorator,
        detail::repeat_while_integral_decorator<FIT_RECURSIVE_CONSTEXPR_DEPTH>
    >
>);

} // namespace fit

#endif
