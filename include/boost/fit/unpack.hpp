/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    unpack.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_UNPACK_H
#define BOOST_FIT_GUARD_UNPACK_H

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
///     using namespace boost::fit;
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

#include <boost/fit/unpack_sequence.hpp>
#include <boost/fit/is_unpackable.hpp>
#include <boost/fit/detail/seq.hpp>
#include <boost/fit/capture.hpp>
#include <boost/fit/always.hpp>
#include <boost/fit/reveal.hpp>
#include <boost/fit/detail/and.hpp>
#include <boost/fit/detail/delegate.hpp>
#include <boost/fit/detail/holder.hpp>
#include <boost/fit/detail/move.hpp>
#include <boost/fit/detail/make.hpp>
#include <boost/fit/detail/static_const_var.hpp>

namespace boost { namespace fit {

namespace detail {

template<class F, class Sequence>
constexpr auto unpack_simple(F&& f, Sequence&& s) BOOST_FIT_RETURNS
(
    detail::unpack_impl(BOOST_FIT_FORWARD(F)(f), BOOST_FIT_FORWARD(Sequence)(s))
)

template<class F, class... Sequences>
constexpr auto unpack_join(F&& f, Sequences&&... s) BOOST_FIT_RETURNS
(
    boost::fit::pack_join(unpack_simple(boost::fit::pack_forward, BOOST_FIT_FORWARD(Sequences)(s))...)(BOOST_FIT_FORWARD(F)(f))
);

}

template<class F>
struct unpack_adaptor : detail::callable_base<F>
{
    typedef unpack_adaptor fit_rewritable1_tag;
    BOOST_FIT_INHERIT_CONSTRUCTOR(unpack_adaptor, detail::callable_base<F>);

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const noexcept
    {
        return boost::fit::always_ref(*this)(xs...);
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

            template<class T, class=typename std::enable_if<(
                is_unpackable<T>::value
            )>::type>
            static auto deduce(T&& x)
            BOOST_FIT_RETURNS
            (
                boost::fit::detail::unpack_simple(deducer(), BOOST_FIT_FORWARD(T)(x))
            );

            template<class T, class... Ts, class=typename std::enable_if<(
                is_unpackable<T>::value && BOOST_FIT_AND_UNPACK(is_unpackable<Ts>::value)
            )>::type>
            static auto deduce(T&& x, Ts&&... xs) BOOST_FIT_RETURNS
            (
                boost::fit::detail::unpack_join(deducer(), BOOST_FIT_FORWARD(T)(x), BOOST_FIT_FORWARD(Ts)(xs)...)
            );
#ifdef _MSC_VER
            template<class... Ts>
            struct nop_failure;
            template<class... Ts, class=typename std::enable_if<(
                !BOOST_FIT_AND_UNPACK(is_unpackable<Ts>::value)
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

    BOOST_FIT_RETURNS_CLASS(unpack_adaptor);
    template<class T, class=typename std::enable_if<(
        is_unpackable<T>::value
    )>::type>
    constexpr auto operator()(T&& x) const
    BOOST_FIT_RETURNS
    (
        boost::fit::detail::unpack_simple(BOOST_FIT_MANGLE_CAST(const detail::callable_base<F>&)(BOOST_FIT_CONST_THIS->base_function(x)), BOOST_FIT_FORWARD(T)(x))
    );

    template<class T, class... Ts, class=typename std::enable_if<(
        is_unpackable<T>::value && BOOST_FIT_AND_UNPACK(is_unpackable<Ts>::value)
    )>::type>
    constexpr auto operator()(T&& x, Ts&&... xs) const BOOST_FIT_RETURNS
    (
        boost::fit::detail::unpack_join(BOOST_FIT_MANGLE_CAST(const detail::callable_base<F>&)(BOOST_FIT_CONST_THIS->base_function(x)), BOOST_FIT_FORWARD(T)(x), BOOST_FIT_FORWARD(Ts)(xs)...)
    );
};

BOOST_FIT_DECLARE_STATIC_VAR(unpack, detail::make<unpack_adaptor>);

}} // namespace boost::fit

#endif
