/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    capture.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_CAPTURE_H
#define FIT_GUARD_CAPTURE_H

#include <fit/detail/callable_base.hpp>
#include <fit/reveal.hpp>
#include <fit/pack.hpp>
#include <fit/always.hpp>
#include <fit/detail/move.hpp>

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
///     auto add_one = fit::capture(1)(sum_f());
///     assert(add_one(2) == 3);
/// 

namespace fit {

namespace detail {

template<class F, class Pack>
struct capture_invoke : detail::callable_base<F>, Pack
{
    typedef capture_invoke fit_rewritable1_tag;
    template<class X, class Y>
    constexpr capture_invoke(X&& x, Y&& y) : detail::callable_base<F>(fit::forward<X>(x)), Pack(fit::forward<Y>(y))
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

    FIT_RETURNS_CLASS(capture_invoke);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT
    (
        typename result_of<decltype(fit::pack_join), 
            id_<const Pack&>, 
            result_of<decltype(fit::pack_forward), id_<Ts>...> 
        >::type,
        id_<detail::callable_base<F>&&>
    ) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        fit::pack_join
        (
            FIT_MANGLE_CAST(const Pack&)(FIT_CONST_THIS->get_pack(xs...)), 
            fit::pack_forward(fit::forward<Ts>(xs)...)
        )
        (FIT_RETURNS_C_CAST(detail::callable_base<F>&&)(FIT_CONST_THIS->base_function(xs...)))
    );
};

template<class Pack>
struct capture_pack : Pack
{
    FIT_INHERIT_CONSTRUCTOR(capture_pack, Pack);

    FIT_RETURNS_CLASS(capture_pack);

    // TODO: Should use rvalue ref qualifier
    template<class F>
    constexpr auto operator()(F f) const FIT_SFINAE_RETURNS
    (
        capture_invoke<F, Pack>(fit::move(f), 
            FIT_RETURNS_C_CAST(Pack&&)(
                FIT_RETURNS_STATIC_CAST(const Pack&)(*always(FIT_CONST_THIS)(f))
            )
        )
    );
};

struct make_capture_pack_f
{
    template<class Pack>
    constexpr capture_pack<Pack> operator()(Pack p) const
    {
        return capture_pack<Pack>(fit::move(p));
    }
};

template<class F>
struct capture_f
{
    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        FIT_RETURNS_CONSTRUCT(make_capture_pack_f)()(FIT_RETURNS_CONSTRUCT(F)()(fit::forward<Ts>(xs)...))
    );
};
}

FIT_DECLARE_STATIC_VAR(capture, detail::capture_f<detail::pack_f>);
FIT_DECLARE_STATIC_VAR(capture_forward, detail::capture_f<detail::pack_forward_f>);
FIT_DECLARE_STATIC_VAR(capture_decay, detail::capture_f<detail::pack_decay_f>);

} // namespace fit

#endif
