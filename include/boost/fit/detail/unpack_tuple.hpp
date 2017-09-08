/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    unpack_tuple.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_UNPACK_TUPLE_HPP
#define BOOST_FIT_GUARD_UNPACK_TUPLE_HPP

#include <boost/fit/unpack_sequence.hpp>
#include <boost/fit/returns.hpp>
#include <boost/fit/detail/forward.hpp>
#include <boost/fit/detail/seq.hpp>
#include <tuple>

namespace boost { namespace fit {

namespace detail {

template<class Sequence>
constexpr typename gens<std::tuple_size<Sequence>::value>::type 
make_tuple_gens(const Sequence&)
{
    return {};
}

#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7)

template<std::size_t I, class Tuple>
struct tuple_element_return
: std::tuple_element<I, Tuple>
{};

template<std::size_t I, class Tuple>
struct tuple_element_return<I, Tuple&>
: std::add_lvalue_reference<typename tuple_element_return<I, Tuple>::type>
{};

template<std::size_t I, class Tuple>
struct tuple_element_return<I, Tuple&&>
: std::add_rvalue_reference<typename tuple_element_return<I, Tuple>::type>
{};

template<std::size_t I, class Tuple>
struct tuple_element_return<I, const Tuple>
: std::add_const<typename tuple_element_return<I, Tuple>::type>
{};

template< std::size_t I, class Tuple, class R = typename tuple_element_return<I, Tuple&&>::type >
R tuple_get( Tuple&& t ) 
{ 
    return (R&&)(std::get<I>(boost::fit::forward<Tuple>(t))); 
}
#define BOOST_FIT_UNPACK_TUPLE_GET boost::fit::detail::tuple_get
#else
#define BOOST_FIT_UNPACK_TUPLE_GET std::get

#endif

template<class F, class T, std::size_t ...N>
constexpr auto unpack_tuple(F&& f, T&& t, seq<N...>) BOOST_FIT_RETURNS
(
    f(
        BOOST_FIT_AUTO_FORWARD(BOOST_FIT_UNPACK_TUPLE_GET<N>(BOOST_FIT_AUTO_FORWARD(t)))...
    )
);

}

template<class... Ts>
struct unpack_sequence<std::tuple<Ts...>>
{
    template<class F, class S>
    constexpr static auto apply(F&& f, S&& t) BOOST_FIT_RETURNS
    (
        boost::fit::detail::unpack_tuple(BOOST_FIT_FORWARD(F)(f), BOOST_FIT_FORWARD(S)(t), boost::fit::detail::make_tuple_gens(t))
    );
};

}} // namespace boost::fit

#endif
