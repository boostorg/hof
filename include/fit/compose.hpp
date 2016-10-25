/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    compose.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_COMPOSE_H
#define FIT_GUARD_FUNCTION_COMPOSE_H

/// compose
/// =======
/// 
/// Description
/// -----------
/// 
/// The `compose` function adaptor provides function composition. It produces
/// a function object that composes a set of functions, ie the output of one
/// function becomes the input of the second function. So, `compose(f, g)(0)`
/// is equivalent to `f(g(0))`.
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class... Fs>
///     constexpr compose_adaptor<Fs...> compose(Fs... fs);
/// 
/// Semantics
/// ---------
/// 
///     assert(compose(f, g)(xs...) == f(g(xs...)));
/// 
/// Requirements
/// ------------
/// 
/// Fs must be:
/// 
/// * [ConstCallable](ConstCallable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
///     using namespace fit;
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
///     int main() {
///         int r = compose(increment(), decrement(), increment())(3);
///         assert(r == 4);
///     }
/// 

#include <fit/detail/callable_base.hpp>
#include <fit/always.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/compressed_pair.hpp>
#include <fit/detail/join.hpp>
#include <tuple>
#include <fit/detail/move.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/result_type.hpp>
#include <fit/detail/static_const_var.hpp>

namespace fit { namespace detail {

template<class F1, class F2>
struct compose_kernel : detail::compressed_pair<F1, F2>, compose_function_result_type<F1, F2>
{
    typedef detail::compressed_pair<F1, F2> base_type;

    FIT_INHERIT_CONSTRUCTOR(compose_kernel, base_type)

    FIT_RETURNS_CLASS(compose_kernel);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(const F1&, result_of<const F2&, id_<Ts>...>) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        FIT_MANGLE_CAST(const F1&)(FIT_CONST_THIS->first(xs...))(
            FIT_MANGLE_CAST(const F2&)(FIT_CONST_THIS->second(xs...))(FIT_FORWARD(Ts)(xs)...)
        )
    );
};
}

template<class F, class... Fs>
struct compose_adaptor 
: detail::compose_kernel<detail::callable_base<F>, FIT_JOIN(compose_adaptor, detail::callable_base<Fs>...)>
{
    typedef compose_adaptor fit_rewritable_tag;
    typedef FIT_JOIN(compose_adaptor, detail::callable_base<Fs>...) tail;
    typedef detail::compose_kernel<detail::callable_base<F>, tail> base_type;

    FIT_INHERIT_DEFAULT(compose_adaptor, base_type)

    template<class X, class... Xs, 
        FIT_ENABLE_IF_CONSTRUCTIBLE(detail::callable_base<F>, X), 
        FIT_ENABLE_IF_CONSTRUCTIBLE(tail, Xs...)
    >
    constexpr compose_adaptor(X&& f1, Xs&& ... fs) 
    : base_type(FIT_FORWARD(X)(f1), tail(FIT_FORWARD(Xs)(fs)...))
    {}

    template<class X,
        FIT_ENABLE_IF_CONSTRUCTIBLE(detail::callable_base<F>, X)
    >
    constexpr compose_adaptor(X&& f1) 
    : base_type(FIT_FORWARD(X)(f1))
    {}
};

template<class F>
struct compose_adaptor<F> : detail::callable_base<F>
{
    typedef compose_adaptor fit_rewritable_tag;

    FIT_INHERIT_DEFAULT(compose_adaptor, detail::callable_base<F>)

    template<class X, FIT_ENABLE_IF_CONVERTIBLE(X, detail::callable_base<F>)>
    constexpr compose_adaptor(X&& f1) 
    : detail::callable_base<F>(FIT_FORWARD(X)(f1))
    {}

};

template<class F1, class F2>
struct compose_adaptor<F1, F2>
: detail::compose_kernel<detail::callable_base<F1>, detail::callable_base<F2>>
{
    typedef compose_adaptor fit_rewritable_tag;
    typedef detail::compose_kernel<detail::callable_base<F1>, detail::callable_base<F2>> base_type;

    FIT_INHERIT_CONSTRUCTOR(compose_adaptor, base_type)
};

FIT_DECLARE_STATIC_VAR(compose, detail::make<compose_adaptor>);

} // namespace fit

#endif
