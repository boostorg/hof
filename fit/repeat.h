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
/// The `repeat` function adaptor will repeatedly apply a function a given
/// number of times.
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class F, class IntegralConstant>
///     constexpr repeat_adaptor<Projection, F> repeat(F f, IntegralConstant);
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
///     FunctionObject
///     MoveConstructible
/// 
/// IntegralConstant must be:
/// 
///     IntegralConstant
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
///     constexpr auto increment_by_5 = fit::repeat(increment(), std::integral_constant<int, 5>());
///     assert(increment_by_5(1) == 6);
/// 

#include <fit/always.h>
#include <fit/detail/delegate.h>
#include <fit/detail/result_of.h>
#include <fit/detail/move.h>
#include <fit/detail/make.h>
#include <fit/detail/sfinae.h>
#include <fit/detail/static_const_var.h>

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

}

template<class F, class T>
struct repeat_adaptor : F
{
    FIT_INHERIT_CONSTRUCTOR(repeat_adaptor, F)

    template<class X, FIT_ENABLE_IF_CONVERTIBLE(X, F)>
    constexpr repeat_adaptor(X&& x, T) : F(fit::forward<X>(x))
    {}

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(repeat_adaptor);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(detail::repeater<T::value>, id_<const F&>, id_<Ts>...) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        detail::repeater<T::value>()
        (
            FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...)), 
            fit::forward<Ts>(xs)...
        )
    );
};

FIT_DECLARE_STATIC_VAR(repeat, detail::make<repeat_adaptor>);

}

#endif