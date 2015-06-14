/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    result_of.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_DETAIL_RESULT_OF_H
#define FIT_GUARD_DETAIL_RESULT_OF_H

#include <fit/returns.h>
#include <fit/detail/sfinae.h>

#ifndef FIT_HAS_MANUAL_DEDUCTION
#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8)
#define FIT_HAS_MANUAL_DEDUCTION 1
#else
#define FIT_HAS_MANUAL_DEDUCTION 0
#endif
#endif

#if FIT_HAS_MANUAL_DEDUCTION || FIT_NO_EXPRESSION_SFINAE

#include <type_traits>
#include <fit/is_callable.h>
#include <fit/detail/holder.h>

namespace fit { namespace detail {

template<class F, class Args, class=void>
struct result_of_impl {};

template<class F, class... Ts>
struct result_of_impl<
    F, 
    holder<Ts...>, 
    typename std::enable_if<is_callable<F, typename Ts::type...>::value>::type
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

}
#endif

#if FIT_NO_EXPRESSION_SFINAE

#define FIT_SFINAE_RESULT(...) typename fit::result_of<__VA_ARGS__>::type
#define FIT_SFINAE_RETURNS(...) { return __VA_ARGS__; }

#else

#define FIT_SFINAE_RESULT(...) auto
#define FIT_SFINAE_RETURNS FIT_RETURNS

#endif

#if FIT_HAS_MANUAL_DEDUCTION

#define FIT_SFINAE_MANUAL_RESULT(...) typename fit::result_of<__VA_ARGS__>::type
#if FIT_HAS_COMPLETE_DECLTYPE && FIT_HAS_MANGLE_OVERLOAD
#define FIT_SFINAE_MANUAL_RETURNS(...) { return (__VA_ARGS__); }
#else
#define FIT_SFINAE_MANUAL_RETURNS(...) { FIT_RETURNS_RETURN(__VA_ARGS__); }
#endif

#else

#define FIT_SFINAE_MANUAL_RESULT FIT_SFINAE_RESULT
#define FIT_SFINAE_MANUAL_RETURNS FIT_SFINAE_RETURNS

#endif

#endif