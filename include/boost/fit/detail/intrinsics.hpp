/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    intrinsics.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_INTRINSICS_HPP
#define BOOST_FIT_GUARD_INTRINSICS_HPP

#include <type_traits>
#include <boost/fit/detail/holder.hpp>

#ifndef BOOST_FIT_NO_STD_DEFAULT_CONSTRUCTIBLE
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define BOOST_FIT_NO_STD_DEFAULT_CONSTRUCTIBLE 1
#else
#define BOOST_FIT_NO_STD_DEFAULT_CONSTRUCTIBLE 0
#endif
#endif

#if defined(__clang__)
#define BOOST_FIT_IS_CONSTRUCTIBLE(...) std::is_constructible<__VA_ARGS__>::value
// #define BOOST_FIT_IS_CONSTRUCTIBLE(...) __is_constructible(__VA_ARGS__)
// #define BOOST_FIT_IS_CONVERTIBLE(...) __is_convertible_to(__VA_ARGS__)
#define BOOST_FIT_IS_CONVERTIBLE(...) std::is_convertible<__VA_ARGS__>::value
#define BOOST_FIT_IS_BASE_OF(...) __is_base_of(__VA_ARGS__)
#define BOOST_FIT_IS_CLASS(...) __is_class(__VA_ARGS__)
#define BOOST_FIT_IS_EMPTY(...) __is_empty(__VA_ARGS__)
#define BOOST_FIT_IS_LITERAL(...) __is_literal(__VA_ARGS__)
#elif defined(__GNUC__)
#define BOOST_FIT_IS_CONSTRUCTIBLE(...) std::is_constructible<__VA_ARGS__>::value
#define BOOST_FIT_IS_CONVERTIBLE(...) std::is_convertible<__VA_ARGS__>::value
#define BOOST_FIT_IS_BASE_OF(...) __is_base_of(__VA_ARGS__)
#define BOOST_FIT_IS_CLASS(...) __is_class(__VA_ARGS__)
#define BOOST_FIT_IS_EMPTY(...) __is_empty(__VA_ARGS__)
#define BOOST_FIT_IS_LITERAL(...) __is_literal_type(__VA_ARGS__)
#elif defined(_MSC_VER)
#define BOOST_FIT_IS_CONSTRUCTIBLE(...) __is_constructible(__VA_ARGS__)
#define BOOST_FIT_IS_CONVERTIBLE(...) __is_convertible_to(__VA_ARGS__)
#define BOOST_FIT_IS_BASE_OF(...) __is_base_of(__VA_ARGS__)
#define BOOST_FIT_IS_CLASS(...) __is_class(__VA_ARGS__)
#define BOOST_FIT_IS_EMPTY(...) __is_empty(__VA_ARGS__)
#define BOOST_FIT_IS_LITERAL(...) std::is_literal_type<__VA_ARGS__>::value
#else
#define BOOST_FIT_IS_CONSTRUCTIBLE(...) std::is_constructible<__VA_ARGS__>::value
#define BOOST_FIT_IS_CONVERTIBLE(...) std::is_convertible<__VA_ARGS__>::value
#define BOOST_FIT_IS_BASE_OF(...) std::is_base_of<__VA_ARGS__>::value
#define BOOST_FIT_IS_CLASS(...) std::is_class<__VA_ARGS__>::value
#define BOOST_FIT_IS_EMPTY(...) std::is_empty<__VA_ARGS__>::value
#define BOOST_FIT_IS_LITERAL(...) std::is_literal_type<__VA_ARGS__>::value
#endif

#if BOOST_FIT_NO_STD_DEFAULT_CONSTRUCTIBLE
#define BOOST_FIT_IS_DEFAULT_CONSTRUCTIBLE(...) boost::fit::detail::is_default_constructible_helper<__VA_ARGS__>::value
#else
#define BOOST_FIT_IS_DEFAULT_CONSTRUCTIBLE BOOST_FIT_IS_CONSTRUCTIBLE
#endif

namespace boost { namespace fit { namespace detail {

template<class T, class=void>
struct is_default_constructible_check
: std::false_type
{};

template<class T>
struct is_default_constructible_check<T, typename holder<
    decltype(T())
>::type>
: std::true_type
{};

template<class T>
struct is_default_constructible_helper
: std::conditional<(std::is_reference<T>::value), 
    std::false_type,
    is_default_constructible_check<T>
>::type
{};

template<class T, class... Xs>
struct is_constructible
: std::is_constructible<T, Xs...>
{};

template<class T>
struct is_constructible<T>
: is_default_constructible_helper<T>
{};

}

}} // namespace boost::fit

#endif
