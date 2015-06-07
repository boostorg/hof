/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    flow.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_FLOW_H
#define FIT_GUARD_FUNCTION_FLOW_H

/// flow
/// ====
/// 
/// Description
/// -----------
/// 
/// The `flow` function adaptor provides function composition. It is useful as
/// an alternative to using the pipe operator `|` when chaining functions. It
/// is similiar to `compose` except the evauluation order is reversed. So,
/// `flow(f, g)(0)` is equivalent to `g(f(0))`.
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class... Fs>
///     constexpr flow_adaptor<Fs...> flow(Fs... fs);
/// 
/// Requirements
/// ------------
/// 
/// Fs must be:
/// 
///     FunctionObject
///     MoveConstructible
/// 
/// Example
/// -------
/// 
///     struct increment
///     {
///         template<class T>
///         T operator()(T x) const
///         {
///             return x + 1;
///         }
///     };
/// 
///     struct decrement
///     {
///         template<class T>
///         T operator()(T x) const
///         {
///             return x - 1;
///         }
///     };
/// 
///     int r = flow(increment(), decrement(), increment())(3);
///     assert(r == 4);
/// 

#include <fit/detail/result_of.h>
#include <fit/always.h>
#include <fit/detail/delegate.h>
#include <fit/detail/compressed_pair.h>
#include <fit/detail/join.h>
#include <tuple>
#include <fit/detail/move.h>
#include <fit/detail/make.h>
#include <fit/detail/static_const_var.h>

namespace fit { namespace detail {

template<class F1, class F2>
struct flow_kernel : detail::compressed_pair<F1, F2>
{
    typedef detail::compressed_pair<F1, F2> base_type;

    FIT_INHERIT_CONSTRUCTOR(flow_kernel, base_type)

    FIT_RETURNS_CLASS(flow_kernel);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(const F2&, result_of<const F1&, id_<Ts>...>) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        FIT_MANGLE_CAST(const F2&)(FIT_CONST_THIS->second(xs...))(
            FIT_MANGLE_CAST(const F1&)(FIT_CONST_THIS->first(xs...))(fit::forward<Ts>(xs)...)
        )
    );
};
}

template<class F, class... Fs>
struct flow_adaptor : detail::flow_kernel<F, FIT_JOIN(flow_adaptor, Fs...)>
{
    typedef flow_adaptor fit_rewritable_tag;
    typedef FIT_JOIN(flow_adaptor, Fs...) tail;
    typedef detail::flow_kernel<F, tail> base;

    constexpr flow_adaptor() {}

    template<class X, class... Xs, FIT_ENABLE_IF_CONVERTIBLE(X, F), FIT_ENABLE_IF_CONSTRUCTIBLE(tail, Xs...)>
    constexpr flow_adaptor(X&& f1, Xs&& ... fs) 
    : base(fit::forward<X>(f1), tail(fit::forward<Xs>(fs)...))
    {}
};

template<class F>
struct flow_adaptor<F> : F
{
    typedef flow_adaptor fit_rewritable_tag;
    constexpr flow_adaptor() {}

    template<class X, FIT_ENABLE_IF_CONVERTIBLE(X, F)>
    constexpr flow_adaptor(X&& f1) 
    : F(fit::forward<X>(f1))
    {}

};

FIT_DECLARE_STATIC_VAR(flow, detail::make<flow_adaptor>);

}

#endif
