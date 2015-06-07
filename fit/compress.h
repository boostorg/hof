/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    compress.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_COMPRESS_H
#define FIT_GUARD_COMPRESS_H

#include <fit/detail/result_of.h>
#include <fit/detail/delegate.h>
#include <fit/detail/compressed_pair.h>
#include <fit/detail/move.h>
#include <fit/detail/make.h>
#include <fit/detail/static_const_var.h>

namespace fit { namespace detail {

struct v_fold
{
    template<class F, class State, class T, class... Ts>
    constexpr FIT_SFINAE_RESULT(const v_fold&, id_<const F&>, result_of<const F&, id_<State>, id_<T>>, id_<Ts>...)
    operator()(const F& f, State&& state, T&& x, Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        (*this)(f, f(fit::forward<State>(state), fit::forward<T>(x)), fit::forward<Ts>(xs)...)
    );

    template<class F, class State, class T>
    constexpr FIT_SFINAE_RESULT(const F&, id_<State>, id_<T>)
    operator()(const F& f, State&& state, T&& x) const FIT_SFINAE_RETURNS
    (
        f(fit::forward<State>(state), fit::forward<T>(x))
    );
};

}

template<class F, class State=void>
struct compress_adaptor
: detail::compressed_pair<F, State>
{
    typedef detail::compressed_pair<F, State> base_type;
    FIT_INHERIT_CONSTRUCTOR(compress_adaptor, base_type)

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

    template<class T, class... Ts>
    constexpr FIT_SFINAE_RESULT(detail::v_fold, id_<const F&>, id_<const State&>, id_<T>, id_<Ts>...)
    operator()(T&& x, Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        detail::v_fold()(
            FIT_MANGLE_CAST(const F&)(this->base_function(x, xs...)), 
            FIT_MANGLE_CAST(const State&)(this->get_state(x, xs...)), 
            fit::forward<T>(x),
            fit::forward<Ts>(xs)...
        )
    )
};


template<class F>
struct compress_adaptor<F>
: F
{
    FIT_INHERIT_CONSTRUCTOR(compress_adaptor, F)

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class T, class... Ts>
    constexpr FIT_SFINAE_RESULT(detail::v_fold, id_<const F&>, id_<T>, id_<Ts>...)
    operator()(T&& x, Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        detail::v_fold()(
            FIT_MANGLE_CAST(const F&)(this->base_function(x, xs...)), 
            fit::forward<T>(x),
            fit::forward<Ts>(xs)...
        )
    )
};

FIT_DECLARE_STATIC_VAR(compress, detail::make<compress_adaptor>);

}

#endif
