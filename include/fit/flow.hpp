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
/// an alternative to using the pipe operator `|` when chaining functions. It is
/// similiar to [`compose`](compose.md) except the evauluation order is
/// reversed. So, `flow(f, g)(0)` is equivalent to `g(f(0))`.
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class... Fs>
///     constexpr flow_adaptor<Fs...> flow(Fs... fs);
/// 
/// Semantics
/// ---------
/// 
///     assert(flow(f, g)(xs...) == g(f(xs...)));
/// 
/// Requirements
/// ------------
/// 
/// Fs must be:
/// 
/// * [Callable](concepts.md#callable)
/// * MoveConstructible
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

#include <fit/detail/callable_base.hpp>
#include <fit/always.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/compressed_pair.hpp>
#include <fit/detail/join.hpp>
#include <tuple>
#include <fit/detail/move.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/static_const_var.hpp>

namespace fit { namespace detail {

template<class F1, class F2>
struct flow_kernel : detail::compressed_pair<detail::callable_base<F1>, detail::callable_base<F2>>
{
    typedef detail::compressed_pair<detail::callable_base<F1>, detail::callable_base<F2>> base_type;

    FIT_INHERIT_CONSTRUCTOR(flow_kernel, base_type)

    FIT_RETURNS_CLASS(flow_kernel);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(const detail::callable_base<F2>&, result_of<const detail::callable_base<F1>&, id_<Ts>...>) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        FIT_MANGLE_CAST(const detail::callable_base<F2>&)(FIT_CONST_THIS->second(xs...))(
            FIT_MANGLE_CAST(const detail::callable_base<F1>&)(FIT_CONST_THIS->first(xs...))(fit::forward<Ts>(xs)...)
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

    FIT_INHERIT_DEFAULT(flow_adaptor, base)

    template<class X, class... Xs, FIT_ENABLE_IF_CONVERTIBLE(X, F), FIT_ENABLE_IF_CONSTRUCTIBLE(tail, Xs...)>
    constexpr flow_adaptor(X&& f1, Xs&& ... fs) 
    : base(fit::forward<X>(f1), tail(fit::forward<Xs>(fs)...))
    {}
};

template<class F>
struct flow_adaptor<F> : detail::callable_base<F>
{
    typedef flow_adaptor fit_rewritable_tag;
    FIT_INHERIT_DEFAULT(flow_adaptor, detail::callable_base<F>)

    template<class X, FIT_ENABLE_IF_CONVERTIBLE(X, detail::callable_base<F>)>
    constexpr flow_adaptor(X&& f1) 
    : detail::callable_base<F>(fit::forward<X>(f1))
    {}

};

FIT_DECLARE_STATIC_VAR(flow, detail::make<flow_adaptor>);

} // namespace fit

#endif
