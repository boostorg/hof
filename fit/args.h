/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    args.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_ARGS_H
#define FIT_GUARD_FUNCTION_ARGS_H

#include <fit/detail/seq.h>
#include <fit/returns.h>
#include <utility>

/// args
/// ====
/// 
/// Description
/// -----------
/// 
/// The `args` function returns the Nth argument passed to it. It actually
/// starts at 1, so it is not the zero-based index of the argument.
/// 
/// Synopsis
/// --------
/// 
///     template<int N, class... Ts>
///     constexpr auto args(Ts&&... xs);
/// 
/// Example
/// -------
/// 
///     assert(args<3>(1,2,3,4,5) == 3);
/// 

namespace fit {

namespace detail {

template<class T>
struct perfect_ref
{
    typedef T&& type;
    T&& value;
    constexpr perfect_ref(T&& x) : value(std::forward<T>(x))
    {}
};

template<class T>
constexpr perfect_ref<T> make_perfect_ref(T&& x)
{
    return { std::forward<T>(x) };
}

template<int N>
struct ignore
{
    template<class T>
    constexpr ignore(T&&...)
    {}
};

template<int... N>
struct args_at
{
    template<class T, class... Ts>
    constexpr auto operator()(ignore<N>..., T x, Ts...) const 
    FIT_RETURNS(std::forward<typename T::type>(x.value));
};

template<int... N>
constexpr args_at<N...> make_args_at(seq<N...>)
{
    return {};
}

}

// TODO: Make this a variable template in C++14
template<int N, class... Ts>
constexpr auto args(Ts&&... xs) FIT_RETURNS
(
    detail::make_args_at(typename detail::gens<N>::type())(nullptr, detail::make_perfect_ref(std::forward<Ts>(xs))...)
);

}

#endif