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
///     constexpr reverse_compress_adaptor<F, State> reverse_compress(F f, State s);
/// 
///     template<class F>
///     constexpr reverse_compress_adaptor<F> reverse_compress(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(reverse_compress(f, z)() == z);
///     assert(reverse_compress(f, z)(x, xs...) == f(reverse_compress(f, z)(xs...), x));
///     assert(reverse_compress(f)(x) == x);
///     assert(reverse_compress(f)(x, xs...) == f(reverse_compress(f)(xs...), x));
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
///         assert(fit::reverse_compress(max_f())(2, 3, 4, 5) == 5);
///     }
/// 
/// References
/// ----------
/// 
/// * [Projections](Projections)
/// * [Variadic print](<Variadic print>)
/// 

#include <fit/detail/builder.hpp>
#include <fit/detail/builder/binary.hpp>
#include <fit/reveal.hpp>

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

struct reverse_compress_binary_adaptor_base
{
    template<class... F>
    struct base
    {
        // TODO: Implement failure
    };

    struct apply 
    {
        template<class F, class State, class... Ts>
        constexpr FIT_SFINAE_RESULT(detail::v_reverse_fold, id_<const F&>, id_<State>, id_<Ts>...)
        operator()(const F& f, State state, Ts&&... xs) const FIT_SFINAE_RETURNS
        (
            detail::v_reverse_fold()(
                f,
                fit::move(state),
                FIT_FORWARD(Ts)(xs)...
            )
        )
    };
};

struct reverse_compress_unary_adaptor_base
{
    template<class... F>
    struct base
    {
        // TODO: Implement failure
    };

    struct apply 
    {
        template<class F, class... Ts>
        constexpr FIT_SFINAE_RESULT(detail::v_reverse_fold, id_<const F&>, id_<Ts>...)
        operator()(const F& f, Ts&&... xs) const FIT_SFINAE_RETURNS
        (
            detail::v_reverse_fold()(
                f,
                FIT_FORWARD(Ts)(xs)...
            )
        )
    };
};

}

template<class F, class... T>
FIT_DECLARE_ADAPTOR_USING(reverse_compress, FIT_BUILDER_JOIN_BASE(
    detail::binary_adaptor_builder<detail::reverse_compress_binary_adaptor_base, detail::reverse_compress_unary_adaptor_base>
)(detail::callable_base<F>, T...))

} // namespace fit

#endif
