/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    reverse_compress.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_REVERSE_COMPRESS_H
#define FIT_GUARD_REVERSE_COMPRESS_H

/// reverse_compress
/// ========
/// 
/// Description
/// -----------
/// 
/// The `reverse_compress` function adaptor uses a binary function to apply a
/// reverse fold(ie right fold in functional programming terms) operation to
/// the arguments passed to the function. Additionally, an optional initial
/// state can be provided, otherwise the first argument is used as the initial
/// state.
/// 
/// The arguments to the binary function, take first the state and then the
/// argument.
/// 
/// Synopsis
/// --------
/// 
///     template<class F, class State>
///     constexpr reverse_compress_adaptor<F, State> reverse_compress(F f, State s);
/// 
///     template<class F>
///     constexpr reverse_compress_adaptor<F> reverse_compress(F f);
/// 
/// Requirements
/// ------------
/// 
/// State must be:
/// 
///     MoveConstructible
/// 
/// F must be:
/// 
///     BinaryFunctionObject
///     MoveConstructible
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
///     assert(fit::reverse_compress(max_f())(2, 3, 4, 5) == 5);
/// 

#include <fit/detail/result_of.h>
#include <fit/detail/delegate.h>
#include <fit/detail/compressed_pair.h>
#include <fit/detail/move.h>
#include <fit/detail/make.h>
#include <fit/detail/static_const_var.h>

namespace fit { namespace detail {

struct v_reverse_fold
{
    FIT_RETURNS_CLASS(v_reverse_fold);
    template<class F, class State, class T, class... Ts>
    constexpr FIT_SFINAE_MANUAL_RESULT(const F&, result_of<const v_reverse_fold&, id_<const F&>, id_<State>, id_<Ts>...>, id_<T>)
    operator()(const F& f, State&& state, T&& x, Ts&&... xs) const FIT_SFINAE_MANUAL_RETURNS
    (
        f((*FIT_CONST_THIS)(f, fit::forward<State>(state), fit::forward<Ts>(xs)...), fit::forward<T>(x))
    );

    template<class F, class State>
    constexpr State operator()(const F&, State&& state) const 
    {
        return fit::forward<State>(state);
    }
};

}

template<class F, class State=void>
struct reverse_compress_adaptor
: detail::compressed_pair<F, State>
{
    typedef detail::compressed_pair<F, State> base_type;
    FIT_INHERIT_CONSTRUCTOR(reverse_compress_adaptor, base_type)

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return this->first(xs...);
    }

    template<class... Ts>
    constexpr const State& get_state(Ts&&... xs) const
    {
        return this->second(xs...);
    }

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(detail::v_reverse_fold, id_<const F&>, id_<const State&>, id_<Ts>...)
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        detail::v_reverse_fold()(
            FIT_MANGLE_CAST(const F&)(this->base_function(xs...)), 
            FIT_MANGLE_CAST(const State&)(this->get_state(xs...)), 
            fit::forward<Ts>(xs)...
        )
    )
};


template<class F>
struct reverse_compress_adaptor<F, void>
: F
{
    FIT_INHERIT_CONSTRUCTOR(reverse_compress_adaptor, F)

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(detail::v_reverse_fold, id_<const F&>, id_<Ts>...)
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        detail::v_reverse_fold()(
            FIT_MANGLE_CAST(const F&)(this->base_function(xs...)), 
            fit::forward<Ts>(xs)...
        )
    )
};

FIT_DECLARE_STATIC_VAR(reverse_compress, detail::make<reverse_compress_adaptor>);

}

#endif
