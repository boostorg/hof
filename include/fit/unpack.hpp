/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    unpack.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_UNPACK_H
#define FIT_GUARD_UNPACK_H

/// unpack
/// ======
/// 
/// Description
/// -----------
/// 
/// The `unpack` function adaptor takes a sequence and uses the elements of
/// the sequence for the arguments to the function. Multiple sequences can be
/// passed to the function. All elements from each sequence will be passed
/// into the function. 
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     unpack_adaptor<F> unpack(F f);
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
///     using namespace fit;
/// 
///     struct sum
///     {
///         template<class T, class U>
///         T operator()(T x, U y) const
///         {
///             return x+y;
///         }
///     };
/// 
///     int main() {
///         int r = unpack(sum())(std::make_tuple(3,2));
///         assert(r == 5);
///     }
/// 
/// 

#include <fit/unpack_sequence.hpp>
#include <fit/is_unpackable.hpp>
#include <fit/detail/seq.hpp>
#include <fit/capture.hpp>
#include <fit/always.hpp>
#include <fit/reveal.hpp>
#include <fit/detail/and.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/holder.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/static_const_var.hpp>

namespace fit {

namespace detail {

template<class F, class Sequence>
constexpr auto unpack_simple(F&& f, Sequence&& s) FIT_RETURNS
(
    detail::unpack_impl(FIT_FORWARD(F)(f), FIT_FORWARD(Sequence)(s))
)

template<class F, class... Sequences>
constexpr auto unpack_join(F&& f, Sequences&&... s) FIT_RETURNS
(
    fit::pack_join(unpack_simple(fit::pack_forward, FIT_FORWARD(Sequences)(s))...)(FIT_FORWARD(F)(f))
);

}

template<class F>
struct unpack_adaptor : detail::callable_base<F>
{
    typedef unpack_adaptor fit_rewritable1_tag;
    FIT_INHERIT_CONSTRUCTOR(unpack_adaptor, detail::callable_base<F>);

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    struct unpack_failure
    {
        template<class Failure>
        struct apply
        {
            struct deducer
            {
                template<class... Ts>
                typename Failure::template of<Ts...> operator()(Ts&&...) const;
            };
#ifdef _MSC_VER
#define FIT_UNPACK_DEDUCE_RETURNS FIT_SFINAE_RETURNS
#else
#define FIT_UNPACK_DEDUCE_RETURNS FIT_RETURNS
#endif

            template<class T, class=typename std::enable_if<(
                is_unpackable<T>::value
            )>::type>
            static auto deduce(T&& x)
            FIT_UNPACK_DEDUCE_RETURNS
            (
                detail::unpack_simple(deducer(), FIT_FORWARD(T)(x))
            );

            template<class T, class... Ts, class=typename std::enable_if<(
                is_unpackable<T>::value && FIT_AND_UNPACK(is_unpackable<Ts>::value)
            )>::type>
            static auto deduce(T&& x, Ts&&... xs) FIT_UNPACK_DEDUCE_RETURNS
            (
                detail::unpack_join(deducer(), FIT_FORWARD(T)(x), FIT_FORWARD(Ts)(xs)...)
            );

            template<class... Ts>
            struct of
#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7) || defined (_MSC_VER)
            : std::enable_if<true, decltype(apply::deduce(std::declval<Ts>()...))>::type
#else
            : decltype(apply::deduce(std::declval<Ts>()...))
#endif
            {};
        };
    };

    struct failure
    : failure_map<unpack_failure, detail::callable_base<F>>
    {};

    FIT_RETURNS_CLASS(unpack_adaptor);
    template<class T, class=typename std::enable_if<(
        is_unpackable<T>::value
    )>::type>
    constexpr auto operator()(T&& x) const
    FIT_RETURNS
    (
        detail::unpack_simple(FIT_MANGLE_CAST(const detail::callable_base<F>&)(FIT_CONST_THIS->base_function(x)), FIT_FORWARD(T)(x))
    );

    template<class T, class... Ts, class=typename std::enable_if<(
        is_unpackable<T>::value && FIT_AND_UNPACK(is_unpackable<Ts>::value)
    )>::type>
    constexpr auto operator()(T&& x, Ts&&... xs) const FIT_RETURNS
    (
        detail::unpack_join(FIT_MANGLE_CAST(const detail::callable_base<F>&)(FIT_CONST_THIS->base_function(x)), FIT_FORWARD(T)(x), FIT_FORWARD(Ts)(xs)...)
    );
};

FIT_DECLARE_STATIC_VAR(unpack, detail::make<unpack_adaptor>);

} // namespace fit

#endif
