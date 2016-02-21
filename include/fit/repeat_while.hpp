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
/// the predicate returns an integral constant that is true. As such, the
/// predicate must be depedently-typed since it is never called at runtime.
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
/// * [FunctionObject](concepts.md#functionobject)
/// * MoveConstructible
/// 
/// Example
/// -------
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
///     constexpr auto increment_until_6 = fit::repeat_while(not_6())(increment());
///     static_assert(std::is_same<six, decltype(increment_until_6(one()))>::value, "Error");
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
struct integral_while_repeater
{
    template<class P, class F, class T, class... Ts, class Self=integral_while_repeater<Depth-1>>
    constexpr auto operator()(bool b, const P& p, const F& f, T&& x, Ts&&... xs) const FIT_RETURNS
    (
        (b) ? 
            Self()(
                p(x, FIT_FORWARD(Ts)(xs)...), 
                p, 
                f, 
                f(x, FIT_FORWARD(Ts)(xs)...)
            ) : 
            FIT_FORWARD(T)(x)
    );
};

template<>
struct integral_while_repeater<0>
{
    template<class P, class F, class T, class Self=integral_while_repeater<0>>
    auto operator()(bool b, const P& p, const F& f, T&& x) const -> decltype(f(x))
    {
        return (b) ? 
            Self()(
                p(x), 
                p, 
                f, 
                f(x)
            ) : 
            FIT_FORWARD(T)(x);
    }
};
struct repeat_while_integral_decorator
{
    template<class P, class F, class... Ts>
    constexpr auto operator()(const P& p, const F& f, Ts&&... xs) const FIT_RETURNS
    (
        integral_while_repeater<FIT_RECURSIVE_CONSTEXPR_DEPTH>()(true, p, f, FIT_FORWARD(Ts)(xs)...)
    );
};

}

FIT_DECLARE_STATIC_VAR(repeat_while, decorate_adaptor<
    fit::conditional_adaptor<
        detail::repeat_while_constant_decorator
        // detail::repeat_while_integral_decorator
    >
>);

} // namespace fit

#endif
