/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    flip.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FLIP_H
#define BOOST_FIT_GUARD_FLIP_H

/// flip
/// ====
/// 
/// Description
/// -----------
/// 
/// The `flip` function adaptor swaps the first two parameters.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     flip_adaptor<F> flip(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(flip(f)(x, y, xs...) == f(y, x, xs...));
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [BinaryCallable](BinaryCallable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
/// 
///     int main() {
///         int r = boost::fit::flip(boost::fit::_ - boost::fit::_)(2, 5);
///         assert(r == 3);
///     }
/// 

#include <boost/fit/detail/callable_base.hpp>
#include <boost/fit/reveal.hpp>
#include <boost/fit/detail/make.hpp>
#include <boost/fit/detail/static_const_var.hpp>

namespace boost { namespace fit {

template<class F>
struct flip_adaptor : detail::callable_base<F>
{
    typedef flip_adaptor fit_rewritable1_tag;
    BOOST_FIT_INHERIT_CONSTRUCTOR(flip_adaptor, detail::callable_base<F>);

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const
    {
        return boost::fit::always_ref(*this)(xs...);
    }

    struct flip_failure
    {
        template<class Failure>
        struct apply
        {
            template<class T, class U, class... Ts>
            struct of
            : Failure::template of<U, T, Ts...>
            {};
        };
    };

    struct failure
    : failure_map<flip_failure, detail::callable_base<F>>
    {};

    BOOST_FIT_RETURNS_CLASS(flip_adaptor);

    template<class T, class U, class... Ts>
    constexpr BOOST_FIT_SFINAE_RESULT(const detail::callable_base<F>&, id_<U>, id_<T>, id_<Ts>...) 
    operator()(T&& x, U&& y, Ts&&... xs) const BOOST_FIT_SFINAE_RETURNS
    (
        (BOOST_FIT_MANGLE_CAST(const detail::callable_base<F>&)(BOOST_FIT_CONST_THIS->base_function(xs...)))
            (BOOST_FIT_FORWARD(U)(y), BOOST_FIT_FORWARD(T)(x), BOOST_FIT_FORWARD(Ts)(xs)...)
    );
};

BOOST_FIT_DECLARE_STATIC_VAR(flip, detail::make<flip_adaptor>);

}} // namespace boost::fit

#endif
