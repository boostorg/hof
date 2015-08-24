/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    repeat_while.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_REPEAT_WHILE_H
#define FIT_GUARD_REPEAT_WHILE_H

#include <fit/always.h>
#include <fit/detail/delegate.h>
#include <fit/detail/result_of.h>
#include <fit/detail/move.h>
#include <fit/detail/make.h>
#include <fit/detail/sfinae.h>
#include <fit/detail/static_const_var.h>

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
            compute_predicate<P, decltype(f(fit::forward<Ts>(xs)...))>::type::value
        >()(f, p, f(fit::forward<Ts>(xs)...))
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

}

template<class F, class P>
struct repeat_while_adaptor : F, P
{
    FIT_INHERIT_CONSTRUCTOR(repeat_while_adaptor, F)

    template<class X, class Y, 
        FIT_ENABLE_IF_CONVERTIBLE(X, F),
        FIT_ENABLE_IF_CONVERTIBLE(Y, P)
    >
    constexpr repeat_while_adaptor(X&& x, Y&& y) 
    : F(fit::forward<X>(x)), P(fit::forward<Y>(y))
    {}

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Ts>
    constexpr const P& base_predicate(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(repeat_while_adaptor);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(
        detail::while_repeater<
            detail::compute_predicate<P, typename result_of<const F&, id_<Ts>...>::type>::type::value
        >, 
        id_<const F&>, id_<const P&>, id_<Ts>...) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        detail::while_repeater<
            detail::compute_predicate<P, decltype(std::declval<F>()(fit::forward<Ts>(xs)...))>::type::value
        >()
        (
            FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...)), 
            FIT_MANGLE_CAST(const P&)(FIT_CONST_THIS->base_predicate(xs...)), 
            fit::forward<Ts>(xs)...
        )
    );
};

FIT_DECLARE_STATIC_VAR(repeat_while, detail::make<repeat_while_adaptor>);

}

#endif