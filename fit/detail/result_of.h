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

#if FIT_NO_EXPRESSION_SFINAE

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

#define FIT_SFINAE_RESULT(...) typename fit::result_of<__VA_ARGS__>::type
#define FIT_SFINAE_RETURNS(...) { return __VA_ARGS__; }

#else

#define FIT_SFINAE_RESULT(...) auto
#define FIT_SFINAE_RETURNS FIT_RETURNS

#endif

#endif