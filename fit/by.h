/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    by.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_ON_H
#define FIT_GUARD_FUNCTION_ON_H

/// by
/// ==
/// 
/// Description
/// -----------
/// 
/// The `by` function adaptor applies a projection onto the parameters of
/// another function. This is useful, for example, to define a function for
/// sorting such that the ordering is based off of the value of one of its
/// member fields.
/// 
/// Synopsis
/// --------
/// 
///     template<class Projection, class F>
///     constexpr by_adaptor<Projection, F> by(Projection p, F f);
/// 
/// Requirements
/// ------------
/// 
/// Projection must be:
/// 
///     UnaryFunctionObject
///     MoveConstructible
/// 
/// F must be:
/// 
///     FunctionObject
///     MoveConstructible
/// 
/// Example
/// -------
/// 
///     struct foo
///     {
///         foo(int x) : x(x)
///         {}
///         int x;
///     };
///     assert(fit::by(std::mem_fn(&foo::x), _ + _)(foo(1), foo(2)) == 3);
/// 



#include <utility>
#include <fit/always.h>
#include <fit/detail/delegate.h>
#include <fit/returns.h>
#include <fit/detail/move.h>
#include <fit/detail/make.h>
#include <fit/detail/static_constexpr.h>

#ifndef FIT_NO_ORDERD_BRACE_INIT
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 9
#define FIT_NO_ORDERD_BRACE_INIT 1
#else
#define FIT_NO_ORDERD_BRACE_INIT 0
#endif 
#endif

#if FIT_NO_ORDERD_BRACE_INIT
#include <fit/pack.h>
#include <fit/capture.h>
#endif

namespace fit {

namespace detail {

struct apply_f
{
    template<class F, class... Ts>
    constexpr auto operator()(F&& f, Ts&&... xs) const FIT_RETURNS
    (f(fit::forward<Ts>(xs)...))
};

template<class R>
struct eval_helper
{
    R result;

    template<class F, class... Ts>
    constexpr eval_helper(const F& f, Ts&&... xs) : result(f(fit::forward<Ts>(xs)...))
    {}

    constexpr R get_result()
    {
        return (R&&)result;
    }
};

#if FIT_NO_ORDERD_BRACE_INIT
template<class F, class Pack, class T, class... Ts>
constexpr auto eval_ordered(const F& f, Pack&& p, T&& x, Ts&&... xs) FIT_RETURNS
(
    eval_ordered(f, pack_join(p, pack(x())), fit::forward<Ts>(xs)...)
);

template<class F, class Pack>
constexpr auto eval_ordered(const F& f, Pack&& p) FIT_RETURNS
(
    p(f)
);
#endif

template<class Projection, class F, class... Ts>
constexpr auto by_eval(const Projection& p, const F& f, Ts&&... xs) FIT_RETURNS
(
#if FIT_NO_ORDERD_BRACE_INIT
    eval_ordered(f, pack(), fit::capture_forward(p, fit::forward<Ts>(xs))(apply_f())...)
#else
    eval_helper<decltype(f(p(fit::forward<Ts>(xs))...))>
        {f, p(fit::forward<Ts>(xs))...}.get_result()
#endif
);

}

template<class Projection, class F=void>
struct by_adaptor : Projection, F
{
    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Ts>
    constexpr const Projection& base_projection(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class P, class G, FIT_ENABLE_IF_CONVERTIBLE(P, Projection), FIT_ENABLE_IF_CONVERTIBLE(G, F)>
    constexpr by_adaptor(P&& p, G&& f) 
    : Projection(fit::forward<P>(p)), F(fit::forward<G>(f))
    {}

    FIT_RETURNS_CLASS(by_adaptor);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        detail::by_eval(
            FIT_MANGLE_CAST(const Projection&)(FIT_CONST_THIS->base_projection(xs...)),
            FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...)),
            fit::forward<Ts>(xs)...
        )
    );
};

template<class Projection>
struct by_adaptor<Projection, void> : Projection
{
    template<class... Ts>
    constexpr const Projection& base_projection(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class P, FIT_ENABLE_IF_CONVERTIBLE(P, Projection)>
    constexpr by_adaptor(P&& p) 
    : Projection(fit::forward<P>(p))
    {}

    FIT_RETURNS_CLASS(by_adaptor);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        (void)std::initializer_list<int>{(
            FIT_MANGLE_CAST(const Projection&)(FIT_CONST_THIS->base_projection(xs...))
            (fit::forward<Ts>(xs)), 0)...}
    );
};

FIT_STATIC_CONSTEXPR detail::make<by_adaptor> by = {};

}
#endif
