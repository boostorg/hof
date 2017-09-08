/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    rotate.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_ROTATE_H
#define BOOST_FIT_GUARD_ROTATE_H

/// rotate
/// ====
/// 
/// Description
/// -----------
/// 
/// The `rotate` function adaptor moves the first parameter to the last
/// parameter.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     rotate_adaptor<F> rotate(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(rotate(f)(x, xs...) == f(xs..., x));
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [ConstCallable](ConstCallable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
/// 
///     int main() {
///         int r = boost::fit::rotate(boost::fit::_ - boost::fit::_)(2, 5);
///         assert(r == 3);
///     }
/// 

#include <boost/fit/detail/result_of.hpp>
#include <boost/fit/reveal.hpp>
#include <boost/fit/detail/make.hpp>
#include <boost/fit/detail/static_const_var.hpp>

namespace boost { namespace fit {

template<class F>
struct rotate_adaptor : detail::callable_base<F>
{
    typedef rotate_adaptor fit_rewritable1_tag;
    BOOST_FIT_INHERIT_CONSTRUCTOR(rotate_adaptor, detail::callable_base<F>);

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const noexcept
    {
        return boost::fit::always_ref(*this)(xs...);
    }

    struct rotate_failure
    {
        template<class Failure>
        struct apply
        {
            template<class T, class... Ts>
            struct of
            : Failure::template of<Ts..., T>
            {};
        };
    };

    struct failure
    : failure_map<rotate_failure, detail::callable_base<F>>
    {};

    BOOST_FIT_RETURNS_CLASS(rotate_adaptor);

    template<class T, class... Ts>
    constexpr BOOST_FIT_SFINAE_RESULT(const detail::callable_base<F>&, id_<Ts>..., id_<T>) 
    operator()(T&& x, Ts&&... xs) const BOOST_FIT_SFINAE_RETURNS
    (
        (BOOST_FIT_MANGLE_CAST(const detail::callable_base<F>&)(BOOST_FIT_CONST_THIS->base_function(xs...)))
            (BOOST_FIT_FORWARD(Ts)(xs)..., BOOST_FIT_FORWARD(T)(x))
    );
};

BOOST_FIT_DECLARE_STATIC_VAR(rotate, detail::make<rotate_adaptor>);

}} // namespace boost::fit

#endif
