/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    substitute.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_HOF_GUARD_FUNCTION_SUBSTITUTE_H
#define BOOST_HOF_GUARD_FUNCTION_SUBSTITUTE_H

/// substitute
/// =======
/// 
/// Description
/// -----------
/// 
/// The `substitute` function adaptor provides substituted composition. This
/// combinator evaluates a set of functions with the same inputs arguments, and
/// then applies composition. So, `substitute(f, g)(x) == f(x)(g(x))`.
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class... Fs>
///     constexpr substitute_adaptor<Fs...> substitute(Fs... fs);
/// 
/// Semantics
/// ---------
/// 
///     assert(substitute(f, g)(xs...) == f(xs...)(g(xs...)));
/// 
/// Requirements
/// ------------
/// 
/// Fs must be:
/// 
/// * [ConstInvocable](ConstInvocable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     #include <boost/hof.hpp>
///     #include <cassert>
///     using namespace boost::hof;
/// 
///     int main() {
///         int r = substitute(always, always)(3);
///         assert(r == 3);
///     }
/// 
/// References
/// ----------
/// 
/// * [Substitution combinator](https://en.wikipedia.org/wiki/Combinatory_logic#Examples_of_combinators)
/// 
/// 

#include <boost/hof/detail/callable_base.hpp>
// #include <boost/hof/always.hpp>
#include <boost/hof/detail/delegate.hpp>
#include <boost/hof/detail/compressed_pair.hpp>
#include <boost/hof/detail/join.hpp>
#include <tuple>
#include <boost/hof/detail/move.hpp>
#include <boost/hof/detail/make.hpp>
#include <boost/hof/detail/result_type.hpp>
#include <boost/hof/detail/static_const_var.hpp>

namespace boost { namespace hof { namespace detail {

template<class F1, class F2>
struct substitute_kernel : detail::compressed_pair<F1, F2>
                         // , compose_function_result_type<F1, F2>
{
    typedef detail::compressed_pair<F1, F2> base_type;

    BOOST_HOF_INHERIT_CONSTRUCTOR(substitute_kernel, base_type)

    BOOST_HOF_RETURNS_CLASS(substitute_kernel);

    template<class... Ts>
    constexpr BOOST_HOF_SFINAE_RESULT(const F1&, result_of<const F2&, id_<Ts>...>)
    operator()(Ts&&... xs) const BOOST_HOF_SFINAE_RETURNS
    (
        BOOST_HOF_MANGLE_CAST(const F1&)(BOOST_HOF_CONST_THIS->first(xs...))(BOOST_HOF_FORWARD(Ts)(xs)...)(
            BOOST_HOF_MANGLE_CAST(const F2&)(BOOST_HOF_CONST_THIS->second(xs...))(BOOST_HOF_FORWARD(Ts)(xs)...)
        )
    );
};
}

template<class F, class... Fs>
struct substitute_adaptor 
: detail::substitute_kernel<detail::callable_base<F>, BOOST_HOF_JOIN(substitute_adaptor, detail::callable_base<Fs>...)>
{
    typedef substitute_adaptor fit_rewritable_tag;
    typedef BOOST_HOF_JOIN(substitute_adaptor, detail::callable_base<Fs>...) tail;
    typedef detail::substitute_kernel<detail::callable_base<F>, tail> base_type;

    BOOST_HOF_INHERIT_DEFAULT(substitute_adaptor, base_type)

    template<class X, class... Xs,
        BOOST_HOF_ENABLE_IF_CONSTRUCTIBLE(detail::callable_base<F>, X),
        BOOST_HOF_ENABLE_IF_CONSTRUCTIBLE(tail, Xs...)
    >
    constexpr substitute_adaptor(X&& f1, Xs&& ... fs)
    BOOST_HOF_NOEXCEPT(BOOST_HOF_IS_NOTHROW_CONSTRUCTIBLE(base_type, X&&, tail) && BOOST_HOF_IS_NOTHROW_CONSTRUCTIBLE(tail, Xs&&...))
    : base_type(BOOST_HOF_FORWARD(X)(f1), tail(BOOST_HOF_FORWARD(Xs)(fs)...))
    {}

    template<class X, BOOST_HOF_ENABLE_IF_CONSTRUCTIBLE(detail::callable_base<F>, X)>
    constexpr substitute_adaptor(X&& f1) 
    BOOST_HOF_NOEXCEPT_CONSTRUCTIBLE(base_type, X&&)
    : base_type(BOOST_HOF_FORWARD(X)(f1))
    {}
};

template<class F>
struct substitute_adaptor<F> : detail::callable_base<F>
{
    typedef substitute_adaptor fit_rewritable_tag;

    BOOST_HOF_INHERIT_DEFAULT(substitute_adaptor, detail::callable_base<F>)

    template<class X, BOOST_HOF_ENABLE_IF_CONVERTIBLE(X, detail::callable_base<F>)>
    constexpr substitute_adaptor(X&& f1)
    BOOST_HOF_NOEXCEPT_CONSTRUCTIBLE(detail::callable_base<F>, X&&)
    : detail::callable_base<F>(BOOST_HOF_FORWARD(X)(f1))
    {}
};

template<class F1, class F2>
struct substitute_adaptor<F1, F2>
: detail::substitute_kernel<detail::callable_base<F1>, detail::callable_base<F2>>
{
    typedef substitute_adaptor fit_rewritable_tag;
    typedef detail::substitute_kernel<detail::callable_base<F1>, detail::callable_base<F2>> base_type;

    BOOST_HOF_INHERIT_CONSTRUCTOR(substitute_adaptor, base_type)
};

BOOST_HOF_DECLARE_STATIC_VAR(substitute, detail::make<substitute_adaptor>);

}} // namespace boost::hof

#endif
