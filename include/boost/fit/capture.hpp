/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    capture.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_CAPTURE_H
#define BOOST_FIT_GUARD_CAPTURE_H

#include <boost/fit/detail/callable_base.hpp>
#include <boost/fit/reveal.hpp>
#include <boost/fit/pack.hpp>
#include <boost/fit/always.hpp>
#include <boost/fit/detail/move.hpp>

/// capture
/// =======
/// 
/// Description
/// -----------
/// 
/// The `capture` function decorator is used to capture values in a function.
/// It provides more flexibility in capturing than the lambda capture list in
/// C++. It provides a way to do move and perfect capturing. The values
/// captured are prepended to the argument list of the function that will be
/// called.
/// 
/// Synopsis
/// --------
/// 
///     // Capture lvalues by reference and rvalues by value.
///     template<class... Ts>
///     constexpr auto capture(Ts&&... xs);
/// 
///     // Capture lvalues by reference and rvalue reference by reference
///     template<class... Ts>
///     constexpr auto capture_forward(Ts&&... xs);
/// 
///     // Capture by decaying each value
///     template<class... Ts>
///     constexpr auto capture_decay(Ts&&... xs);
/// 
/// Semantics
/// ---------
/// 
///     assert(capture(xs...)(f)(ys...) == f(xs..., ys...));
/// 
/// 
/// Example
/// -------
/// 
///     struct sum_f
///     {
///         template<class T, class U>
///         T operator()(T x, U y) const
///         {
///             return x+y;
///         }
///     };
///     auto add_one = boost::fit::capture(1)(sum_f());
///     assert(add_one(2) == 3);
/// 

namespace boost { namespace fit {

namespace detail {

template<class F, class Pack>
struct capture_invoke : detail::callable_base<F>, Pack
{
    typedef capture_invoke fit_rewritable1_tag;
    template<class X, class Y>
    constexpr capture_invoke(X&& x, Y&& y) : detail::callable_base<F>(BOOST_FIT_FORWARD(X)(x)), Pack(BOOST_FIT_FORWARD(Y)(y))
    {}
    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Ts>
    constexpr const Pack& get_pack(Ts&&...xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class Failure, class... Ts>
    struct unpack_capture_failure
    {
        template<class... Us>
        struct apply
        {
            typedef typename Failure::template of<Us..., Ts...> type;
        };
    };

    struct capture_failure
    {
        template<class Failure>
        struct apply
        {
            template<class... Ts>
            struct of
            : Pack::template apply<unpack_capture_failure<Failure, Ts...>>::type
            {};
        };
    };

    struct failure
    : failure_map<capture_failure, detail::callable_base<F>>
    {};

    BOOST_FIT_RETURNS_CLASS(capture_invoke);

    template<class... Ts>
    constexpr BOOST_FIT_SFINAE_RESULT
    (
        typename result_of<decltype(boost::fit::pack_join), 
            id_<const Pack&>, 
            result_of<decltype(boost::fit::pack_forward), id_<Ts>...> 
        >::type,
        id_<detail::callable_base<F>&&>
    ) 
    operator()(Ts&&... xs) const BOOST_FIT_SFINAE_RETURNS
    (
        boost::fit::pack_join
        (
            BOOST_FIT_MANGLE_CAST(const Pack&)(BOOST_FIT_CONST_THIS->get_pack(xs...)), 
            boost::fit::pack_forward(BOOST_FIT_FORWARD(Ts)(xs)...)
        )
        (BOOST_FIT_RETURNS_C_CAST(detail::callable_base<F>&&)(BOOST_FIT_CONST_THIS->base_function(xs...)))
    );
};

template<class Pack>
struct capture_pack : Pack
{
    BOOST_FIT_INHERIT_CONSTRUCTOR(capture_pack, Pack);

    BOOST_FIT_RETURNS_CLASS(capture_pack);

    // TODO: Should use rvalue ref qualifier
    template<class F>
    constexpr auto operator()(F f) const BOOST_FIT_SFINAE_RETURNS
    (
        capture_invoke<F, Pack>(BOOST_FIT_RETURNS_STATIC_CAST(F&&)(f), 
            BOOST_FIT_RETURNS_C_CAST(Pack&&)(
                BOOST_FIT_RETURNS_STATIC_CAST(const Pack&)(*always(BOOST_FIT_CONST_THIS)(f))
            )
        )
    );
};

struct make_capture_pack_f
{
    template<class Pack>
    constexpr capture_pack<Pack> operator()(Pack p) const
    {
        return capture_pack<Pack>(static_cast<Pack&&>(p));
    }
};

template<class F>
struct capture_f
{
    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const BOOST_FIT_RETURNS
    (
        BOOST_FIT_RETURNS_CONSTRUCT(make_capture_pack_f)()(BOOST_FIT_RETURNS_CONSTRUCT(F)()(BOOST_FIT_FORWARD(Ts)(xs)...))
    );
};
}

BOOST_FIT_DECLARE_STATIC_VAR(capture, detail::capture_f<detail::pack_f>);
BOOST_FIT_DECLARE_STATIC_VAR(capture_forward, detail::capture_f<detail::pack_forward_f>);
BOOST_FIT_DECLARE_STATIC_VAR(capture_decay, detail::capture_f<detail::pack_decay_f>);

}} // namespace boost::fit

#endif
