/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    compose.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FUNCTION_COMPOSE_H
#define BOOST_FIT_GUARD_FUNCTION_COMPOSE_H

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
/// * [ConstCallable](concepts.md#constcallable)
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
///     int r = compose(increment(), decrement(), increment())(3);
///     assert(r == 4);
/// 

#include <boost/fit/detail/callable_base.hpp>
#include <boost/fit/always.hpp>
#include <boost/fit/detail/delegate.hpp>
#include <boost/fit/detail/compressed_pair.hpp>
#include <boost/fit/detail/join.hpp>
#include <tuple>
#include <boost/fit/detail/move.hpp>
#include <boost/fit/detail/make.hpp>
#include <boost/fit/detail/static_const_var.hpp>

namespace boost { namespace fit { namespace detail {

template<class F1, class F2>
struct compose_kernel : detail::compressed_pair<F1, F2>
{
    typedef detail::compressed_pair<F1, F2> base_type;

    BOOST_FIT_INHERIT_CONSTRUCTOR(compose_kernel, base_type)

    BOOST_FIT_RETURNS_CLASS(compose_kernel);

    template<class... Ts>
    constexpr BOOST_FIT_SFINAE_RESULT(const F1&, result_of<const F2&, id_<Ts>...>) 
    operator()(Ts&&... xs) const BOOST_FIT_SFINAE_RETURNS
    (
        BOOST_FIT_MANGLE_CAST(const F1&)(BOOST_FIT_CONST_THIS->first(xs...))(
            BOOST_FIT_MANGLE_CAST(const F2&)(BOOST_FIT_CONST_THIS->second(xs...))(BOOST_FIT_FORWARD(Ts)(xs)...)
        )
    );
};
}

template<class F, class... Fs>
struct compose_adaptor : detail::compose_kernel<detail::callable_base<F>, BOOST_FIT_JOIN(compose_adaptor, detail::callable_base<Fs>...)>
{
    typedef compose_adaptor fit_rewritable_tag;
    typedef BOOST_FIT_JOIN(compose_adaptor, detail::callable_base<Fs>...) tail;
    typedef detail::compose_kernel<detail::callable_base<F>, tail> base_type;

    BOOST_FIT_INHERIT_DEFAULT(compose_adaptor, base_type)

    template<class X, class... Xs, 
        BOOST_FIT_ENABLE_IF_CONSTRUCTIBLE(detail::callable_base<F>, X), 
        BOOST_FIT_ENABLE_IF_CONSTRUCTIBLE(tail, Xs...)
    >
    constexpr compose_adaptor(X&& f1, Xs&& ... fs) 
    : base_type(BOOST_FIT_FORWARD(X)(f1), tail(BOOST_FIT_FORWARD(Xs)(fs)...))
    {}

    template<class X,
        BOOST_FIT_ENABLE_IF_CONSTRUCTIBLE(detail::callable_base<F>, X)
    >
    constexpr compose_adaptor(X&& f1) 
    : base_type(BOOST_FIT_FORWARD(X)(f1))
    {}
};

template<class F>
struct compose_adaptor<F> : detail::callable_base<F>
{
    typedef compose_adaptor fit_rewritable_tag;

    BOOST_FIT_INHERIT_DEFAULT(compose_adaptor, detail::callable_base<F>)

    template<class X, BOOST_FIT_ENABLE_IF_CONVERTIBLE(X, detail::callable_base<F>)>
    constexpr compose_adaptor(X&& f1) 
    : detail::callable_base<F>(BOOST_FIT_FORWARD(X)(f1))
    {}

};

template<class F1, class F2>
struct compose_adaptor<F1, F2>
: detail::compose_kernel<detail::callable_base<F1>, detail::callable_base<F2>>
{
    typedef compose_adaptor fit_rewritable_tag;
    typedef detail::compose_kernel<detail::callable_base<F1>, detail::callable_base<F2>> base_type;

    BOOST_FIT_INHERIT_CONSTRUCTOR(compose_adaptor, base_type)
};

BOOST_FIT_DECLARE_STATIC_VAR(compose, detail::make<compose_adaptor>);

}} // namespace boost::fit

#endif
