/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    reverse_fold.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_REVERSE_FOLD_H
#define FIT_GUARD_REVERSE_FOLD_H

/// reverse_fold
/// ========
/// 
/// Description
/// -----------
/// 
/// The `reverse_fold` function adaptor uses a binary function to apply a
/// reverse [fold]
/// (https://en.wikipedia.org/wiki/Fold_%28higher-order_function%29)(ie right
/// fold in functional programming terms) operation to the arguments passed to
/// the function. Additionally, an optional initial state can be provided,
/// otherwise the first argument is used as the initial state.
/// 
/// The arguments to the binary function, take first the state and then the
/// argument.
/// 
/// Synopsis
/// --------
/// 
///     template<class F, class State>
///     constexpr reverse_fold_adaptor<F, State> reverse_fold(F f, State s);
/// 
///     template<class F>
///     constexpr reverse_fold_adaptor<F> reverse_fold(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(reverse_fold(f, z)() == z);
///     assert(reverse_fold(f, z)(x, xs...) == f(reverse_fold(f, z)(xs...), x));
///     assert(reverse_fold(f)(x) == x);
///     assert(reverse_fold(f)(x, xs...) == f(reverse_fold(f)(xs...), x));
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
/// * [BinaryCallable](BinaryCallable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
/// 
///     struct max_f
///     {
///         template<class T, class U>
///         constexpr T operator()(T x, U y) const
///         {
///             return x > y ? x : y;
///         }
///     };
/// 
///     int main() {
///         assert(fit::reverse_fold(max_f())(2, 3, 4, 5) == 5);
///     }
/// 
/// References
/// ----------
/// 
/// * [Projections](Projections)
/// * [Variadic print](<Variadic print>)
/// 

#include <fit/detail/callable_base.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/compressed_pair.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/static_const_var.hpp>

namespace fit { namespace detail {

struct v_reverse_fold
{
    FIT_RETURNS_CLASS(v_reverse_fold);
    template<class F, class State, class T, class... Ts>
    constexpr FIT_SFINAE_MANUAL_RESULT(const F&, result_of<const v_reverse_fold&, id_<const F&>, id_<State>, id_<Ts>...>, id_<T>)
    operator()(const F& f, State&& state, T&& x, Ts&&... xs) const FIT_SFINAE_MANUAL_RETURNS
    (
        f((*FIT_CONST_THIS)(f, FIT_FORWARD(State)(state), FIT_FORWARD(Ts)(xs)...), FIT_FORWARD(T)(x))
    );

    template<class F, class State>
    constexpr State operator()(const F&, State&& state) const noexcept
    {
        return FIT_FORWARD(State)(state);
    }
};

}

template<class F, class State=void>
struct reverse_fold_adaptor
: detail::compressed_pair<detail::callable_base<F>, State>
{
    typedef detail::compressed_pair<detail::callable_base<F>, State> base_type;
    FIT_INHERIT_CONSTRUCTOR(reverse_fold_adaptor, base_type)

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const noexcept
    {
        return this->first(xs...);
    }

    template<class... Ts>
    constexpr State get_state(Ts&&... xs) const noexcept
    {
        return this->second(xs...);
    }

    FIT_RETURNS_CLASS(reverse_fold_adaptor);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(detail::v_reverse_fold, id_<const detail::callable_base<F>&>, id_<State>, id_<Ts>...)
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        detail::v_reverse_fold()(
            FIT_MANGLE_CAST(const detail::callable_base<F>&)(FIT_CONST_THIS->base_function(xs...)), 
            FIT_MANGLE_CAST(State)(FIT_CONST_THIS->get_state(xs...)), 
            FIT_FORWARD(Ts)(xs)...
        )
    )
};


template<class F>
struct reverse_fold_adaptor<F, void>
: detail::callable_base<F>
{
    FIT_INHERIT_CONSTRUCTOR(reverse_fold_adaptor, detail::callable_base<F>)

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const noexcept
    {
        return fit::always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(reverse_fold_adaptor);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(detail::v_reverse_fold, id_<const detail::callable_base<F>&>, id_<Ts>...)
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        detail::v_reverse_fold()(
            FIT_MANGLE_CAST(const detail::callable_base<F>&)(FIT_CONST_THIS->base_function(xs...)), 
            FIT_FORWARD(Ts)(xs)...
        )
    )
};

FIT_DECLARE_STATIC_VAR(reverse_fold, detail::make<reverse_fold_adaptor>);

} // namespace fit

#endif
