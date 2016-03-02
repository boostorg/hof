/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    result_of.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_DETAIL_RESULT_OF_H
#define BOOST_FIT_GUARD_DETAIL_RESULT_OF_H

#include <boost/fit/returns.hpp>
#include <boost/fit/detail/sfinae.hpp>

#ifndef BOOST_FIT_HAS_MANUAL_DEDUCTION
#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8)
#define BOOST_FIT_HAS_MANUAL_DEDUCTION 1
#else
#define BOOST_FIT_HAS_MANUAL_DEDUCTION 0
#endif
#endif

#if BOOST_FIT_HAS_MANUAL_DEDUCTION || BOOST_FIT_NO_EXPRESSION_SFINAE

#include <boost/fit/detail/and.hpp>
#include <boost/fit/detail/holder.hpp>
#include <boost/fit/detail/can_be_called.hpp>

namespace boost { namespace fit { namespace detail {

template<class F, class Args, class=void>
struct result_of_impl {};

template<class F, class... Ts>
struct result_of_impl<
    F, 
    holder<Ts...>, 
    typename std::enable_if<can_be_called<F, typename Ts::type...>::value>::type
>
{
    typedef decltype(std::declval<F>()(std::declval<typename Ts::type>()...)) type;
};
}

template<class T>
struct id_
{
    typedef T type;
};

template<class F, class... Ts>
struct result_of
: detail::result_of_impl<F, detail::holder<Ts...>>
{};

// template<class F, class... Ts>
// using result_of = detail::result_of_impl<F, detail::holder<Ts...>>;
// using result_of = id_<decltype(std::declval<F>()(std::declval<typename Ts::type>()...))>;

}} // namespace boost::fit
#endif

#if BOOST_FIT_NO_EXPRESSION_SFINAE

#define BOOST_FIT_SFINAE_RESULT(...) typename boost::fit::result_of<__VA_ARGS__>::type
#define BOOST_FIT_SFINAE_RETURNS(...) { return __VA_ARGS__; }

#else

#define BOOST_FIT_SFINAE_RESULT(...) auto
#define BOOST_FIT_SFINAE_RETURNS BOOST_FIT_RETURNS

#endif

#if BOOST_FIT_HAS_MANUAL_DEDUCTION

#define BOOST_FIT_SFINAE_MANUAL_RESULT(...) typename boost::fit::result_of<__VA_ARGS__>::type
#if BOOST_FIT_HAS_COMPLETE_DECLTYPE && BOOST_FIT_HAS_MANGLE_OVERLOAD
#define BOOST_FIT_SFINAE_MANUAL_RETURNS(...) { return (__VA_ARGS__); }
#else
#define BOOST_FIT_SFINAE_MANUAL_RETURNS(...) { BOOST_FIT_RETURNS_RETURN(__VA_ARGS__); }
#endif

#else

#define BOOST_FIT_SFINAE_MANUAL_RESULT BOOST_FIT_SFINAE_RESULT
#define BOOST_FIT_SFINAE_MANUAL_RETURNS BOOST_FIT_SFINAE_RETURNS

#endif

#endif
