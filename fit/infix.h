/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    infix.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_INFIX_H
#define FIT_GUARD_FUNCTION_INFIX_H

/// infix
/// =====
/// 
/// Description
/// -----------
/// 
/// The `infix` function adaptor allows the function to be used as an infix
/// operator. The operator must be placed inside the angle brackets(ie `<`
/// and `>`).
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr infix_adaptor<F> infix(F f);
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
///     BinaryFunctionObject
///     MoveConstructible
/// 
/// Example
/// -------
/// 
///     struct plus_f
///     {
///         template<class T, class U>
///         T operator()(T x, U y) const
///         {
///             return x+y;
///         }
///     };
///     
///     constexpr infix_adaptor<plus_f> plus = {};
///     int r = 3 <plus> 2;
///     assert(r == 5);
/// 

#include <fit/detail/delegate.h>
#include <fit/returns.h>
#include <fit/always.h>

namespace fit {
 
namespace detail{
template<class T, class F>
struct postfix_adaptor : F
{
    T x;

    template<class X, class XF>
    constexpr postfix_adaptor(X&& x, XF&& f) 
    : F(std::forward<XF>(f)), x(std::forward<X>(x))
    {}

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(postfix_adaptor);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        (FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...)))(FIT_MANGLE_CAST(T&&)(std::move(FIT_CONST_THIS->x)), std::forward<Ts>(xs)...)
    );

    template<class A>
    constexpr auto operator>(A&& a) const FIT_RETURNS
    (
        (FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(a)))(FIT_MANGLE_CAST(T&&)(std::move(FIT_CONST_THIS->x)), std::forward<A>(a))
    );
};

template<class T, class F>
constexpr postfix_adaptor<T, F> make_postfix_adaptor(T&& x, F f)
{
    return postfix_adaptor<T, F>(std::forward<T>(x), std::move(f));
}
}

template<class F>
struct infix_adaptor : F
{
    FIT_INHERIT_CONSTRUCTOR(infix_adaptor, F);

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(infix_adaptor);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        (FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...)))(std::forward<Ts>(xs)...)
    );
};

template<class T, class F>
constexpr auto operator<(T&& x, const infix_adaptor<F>& i) FIT_RETURNS
(detail::make_postfix_adaptor(std::forward<T>(x), std::move(i.base_function(x))));

template<class F>
constexpr infix_adaptor<F> infix(F f)
{
    return infix_adaptor<F>(std::move(f));
}


}

#endif