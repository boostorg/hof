/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    compress.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_COMPRESS_H
#define BOOST_FIT_GUARD_COMPRESS_H

/// compress
/// ========
/// 
/// Description
/// -----------
/// 
/// The `compress` function adaptor uses a binary function to apply a
/// [fold](https://en.wikipedia.org/wiki/Fold_%28higher-order_function%29)
/// operation to the arguments passed to the function. Additionally, an
/// optional initial state can be provided, otherwise the first argument is
/// used as the initial state.
/// 
/// The arguments to the binary function, take first the state and then the
/// argument.
/// 
/// Synopsis
/// --------
/// 
///     template<class F, class State>
///     constexpr compress_adaptor<F, State> compress(F f, State s);
/// 
///     template<class F>
///     constexpr compress_adaptor<F> compress(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(compress(f, z)() == z);
///     assert(compress(f, z)(x, xs...) == compress(f, f(z, x))(xs...));
///     assert(compress(f)(x) == x);
///     assert(compress(f)(x, y, xs...) == compress(f)(f(x, y), xs...));
/// 
/// Requirements
/// ------------
/// 
/// State must be:
/// 
/// * CopyConstructible
/// 
/// F must be:
/// 
/// * [BinaryCallable](concepts.md#binarycallable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     struct max_f
///     {
///         template<class T, class U>
///         constexpr T operator()(T x, U y) const
///         {
///             return x > y ? x : y;
///         }
///     };
///     assert(boost::fit::compress(max_f())(2, 3, 4, 5) == 5);
/// 

#include <boost/fit/detail/callable_base.hpp>
#include <boost/fit/detail/delegate.hpp>
#include <boost/fit/detail/compressed_pair.hpp>
#include <boost/fit/detail/move.hpp>
#include <boost/fit/detail/make.hpp>
#include <boost/fit/detail/static_const_var.hpp>

namespace boost { namespace fit { namespace detail {

struct v_fold
{
    BOOST_FIT_RETURNS_CLASS(v_fold);
    template<class F, class State, class T, class... Ts>
    constexpr BOOST_FIT_SFINAE_MANUAL_RESULT(const v_fold&, id_<const F&>, result_of<const F&, id_<State>, id_<T>>, id_<Ts>...)
    operator()(const F& f, State&& state, T&& x, Ts&&... xs) const BOOST_FIT_SFINAE_MANUAL_RETURNS
    (
        (*BOOST_FIT_CONST_THIS)(f, f(BOOST_FIT_FORWARD(State)(state), BOOST_FIT_FORWARD(T)(x)), BOOST_FIT_FORWARD(Ts)(xs)...)
    );

    template<class F, class State>
    constexpr State operator()(const F&, State&& state) const 
    {
        return BOOST_FIT_FORWARD(State)(state);
    }
};

}

template<class F, class State=void>
struct compress_adaptor
: detail::compressed_pair<detail::callable_base<F>, State>
{
    typedef detail::compressed_pair<detail::callable_base<F>, State> base_type;
    BOOST_FIT_INHERIT_CONSTRUCTOR(compress_adaptor, base_type)

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const
    {
        return this->first(xs...);
    }

    template<class... Ts>
    constexpr State get_state(Ts&&... xs) const
    {
        return this->second(xs...);
    }

    template<class... Ts>
    constexpr BOOST_FIT_SFINAE_RESULT(detail::v_fold, id_<const detail::callable_base<F>&>, id_<State>, id_<Ts>...)
    operator()(Ts&&... xs) const BOOST_FIT_SFINAE_RETURNS
    (
        detail::v_fold()(
            BOOST_FIT_MANGLE_CAST(const detail::callable_base<F>&)(this->base_function(xs...)), 
            BOOST_FIT_MANGLE_CAST(State)(this->get_state(xs...)), 
            BOOST_FIT_FORWARD(Ts)(xs)...
        )
    )
};


template<class F>
struct compress_adaptor<F, void>
: detail::callable_base<F>
{
    BOOST_FIT_INHERIT_CONSTRUCTOR(compress_adaptor, detail::callable_base<F>)

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Ts>
    constexpr BOOST_FIT_SFINAE_RESULT(detail::v_fold, id_<const detail::callable_base<F>&>, id_<Ts>...)
    operator()(Ts&&... xs) const BOOST_FIT_SFINAE_RETURNS
    (
        detail::v_fold()(
            BOOST_FIT_MANGLE_CAST(const detail::callable_base<F>&)(this->base_function(xs...)), 
            BOOST_FIT_FORWARD(Ts)(xs)...
        )
    )
};

BOOST_FIT_DECLARE_STATIC_VAR(compress, detail::make<compress_adaptor>);

}} // namespace boost::fit

#endif
