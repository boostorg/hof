/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    capture.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_CAPTURE_H
#define FIT_GUARD_CAPTURE_H

#include <fit/pack.h>
#include <fit/always.h>
#include <fit/detail/move.h>

/// capture
/// =======
/// 
/// Description
/// -----------
/// 
/// The `capture` function is used to capture values in a function. It
/// provides more flexibility in capturing than the lambda capture list in
/// C++. It provides a way to do move and perfect capturing. The values
/// captured are prepended to the argument list of the function that will be
/// called.
/// 
/// Synopsis
/// --------
/// 
///     // Capture lvalues and rvalues
///     template<class... Ts>
///     constexpr auto capture(Ts&&... xs);
/// 
///     // Capture using perfect forwarding
///     template<class... Ts>
///     constexpr auto capture_forward(Ts&&... xs);
/// 
///     // Capture by decaying each value
///     template<class... Ts>
///     constexpr auto capture_decay(Ts&&... xs);
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
struct capture_invoke : F, Pack
{
    template<class X, class Y>
    constexpr capture_invoke(X&& x, Y&& y) : F(fit::forward<X>(x)), Pack(fit::forward<Y>(y))
    {}
    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Ts>
    constexpr const Pack& get_pack(Ts&&...xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(capture_invoke);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        fit::pack_join
        (
            FIT_MANGLE_CAST(const Pack&)(FIT_CONST_THIS->get_pack(xs...)), 
            fit::pack_forward(fit::forward<Ts>(xs)...)
        )
        (FIT_RETURNS_C_CAST(F&&)(FIT_CONST_THIS->base_function(xs...)))
    );
};

template<class Pack>
struct capture_pack : Pack
{
    FIT_INHERIT_CONSTRUCTOR(capture_pack, Pack);

    FIT_RETURNS_CLASS(capture_pack);

    // TODO: Should use rvalue ref qualifier
    template<class F>
    constexpr auto operator()(F f) const FIT_RETURNS
    (
        capture_invoke<F, Pack>(fit::move(f), 
            FIT_RETURNS_C_CAST(Pack&&)(
                FIT_RETURNS_STATIC_CAST(const Pack&)(*always(FIT_CONST_THIS)(f))
            )
        )
    );
};

template<class Pack>
constexpr capture_pack<Pack> make_capture_pack(Pack p)
{
    return capture_pack<Pack>(fit::move(p));
}
}

template<class... Ts>
constexpr auto capture(Ts&&... xs) FIT_RETURNS
(detail::make_capture_pack(pack(fit::forward<Ts>(xs)...)));

template<class... Ts>
constexpr auto capture_forward(Ts&&... xs) FIT_RETURNS
(detail::make_capture_pack(pack_forward(fit::forward<Ts>(xs)...)));

template<class... Ts>
constexpr auto capture_decay(Ts&&... xs) FIT_RETURNS
(detail::make_capture_pack(pack_decay(fit::forward<Ts>(xs)...)));

}

#endif