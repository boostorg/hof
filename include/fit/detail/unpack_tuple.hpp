/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    unpack_tuple.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_UNPACK_TUPLE_HPP
#define FIT_GUARD_UNPACK_TUPLE_HPP

#include <fit/unpack_sequence.hpp>
#include <fit/returns.hpp>
#include <fit/detail/forward.hpp>
#include <fit/detail/seq.hpp>
#include <tuple>

namespace fit {

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
    return (R&&)(std::get<I>(fit::forward<Tuple>(t))); 
}
#define FIT_UNPACK_TUPLE_GET fit::detail::tuple_get
#else
#define FIT_UNPACK_TUPLE_GET std::get

#endif

template<class F, class T, std::size_t ...N>
constexpr auto unpack_tuple(F&& f, T&& t, seq<N...>) FIT_RETURNS
(
    f(
        FIT_AUTO_FORWARD(FIT_UNPACK_TUPLE_GET<N>(FIT_AUTO_FORWARD(t)))...
    )
);

}

template<class... Ts>
struct unpack_sequence<std::tuple<Ts...>>
{
    template<class F, class S>
    constexpr static auto apply(F&& f, S&& t) FIT_RETURNS
    (
        fit::detail::unpack_tuple(FIT_FORWARD(F)(f), FIT_FORWARD(S)(t), fit::detail::make_tuple_gens(t))
    );
};

} // namespace fit

#endif
