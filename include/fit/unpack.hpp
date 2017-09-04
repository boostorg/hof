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
/// References
/// ----------
/// 
/// * [std::apply](http://en.cppreference.com/w/cpp/utility/apply) - C++17 function to unpack a tuple
/// * [`unpack_sequence`](unpack_sequence)
/// 

#include <fit/unpack_sequence.hpp>
#include <fit/is_unpackable.hpp>
#include <fit/detail/seq.hpp>
#include <fit/capture.hpp>
#include <fit/detail/builder.hpp>
#include <fit/detail/builder/unary.hpp>
#include <fit/reveal.hpp>

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

struct unpack_adaptor_builder
{
    template<class F>
    struct base
    {
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

                template<class T, class=typename std::enable_if<(
                    is_unpackable<T>::value
                )>::type>
                static auto deduce(T&& x)
                FIT_RETURNS
                (
                    detail::unpack_simple(deducer(), FIT_FORWARD(T)(x))
                );

                template<class T, class... Ts, class=typename std::enable_if<(
                    is_unpackable<T>::value && FIT_AND_UNPACK(is_unpackable<Ts>::value)
                )>::type>
                static auto deduce(T&& x, Ts&&... xs) FIT_RETURNS
                (
                    detail::unpack_join(deducer(), FIT_FORWARD(T)(x), FIT_FORWARD(Ts)(xs)...)
                );
#ifdef _MSC_VER
                template<class... Ts>
                struct nop_failure;
                template<class... Ts, class=typename std::enable_if<(
                    !FIT_AND_UNPACK(is_unpackable<Ts>::value)
                )>::type>
                static as_failure<nop_failure> deduce(Ts&&... xs);
#endif
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
    };

    struct apply
    {
        template<class F, class T, class=typename std::enable_if<(
            is_unpackable<T>::value
        )>::type>
        constexpr auto operator()(F&& f, T&& x) const
        FIT_RETURNS
        (
            detail::unpack_simple(FIT_FORWARD(F)(f), FIT_FORWARD(T)(x))
        );

        template<class F, class T, class... Ts, class=typename std::enable_if<(
            is_unpackable<T>::value && FIT_AND_UNPACK(is_unpackable<Ts>::value)
        )>::type>
        constexpr auto operator()(F&& f, T&& x, Ts&&... xs) const FIT_RETURNS
        (
            detail::unpack_join(FIT_FORWARD(F)(f), FIT_FORWARD(T)(x), FIT_FORWARD(Ts)(xs)...)
        );
    };

};

}

FIT_DECLARE_ADAPTOR(unpack, detail::unary_adaptor_builder<detail::unpack_adaptor_builder>)

} // namespace fit

#endif
