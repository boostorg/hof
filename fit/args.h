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
/// The `args` returns a function object that returns the Nth argument passed
/// to it. It actually starts at 1, so it is not the zero-based index of the
/// argument.
/// 
/// Synopsis
/// --------
/// 
///     template<class IntegralConstant>
///     constexpr auto args(IntegralConstant);
/// 
/// 
/// Example
/// -------
/// 
///     assert(args(std::integral_constant<int, 3>())(1,2,3,4,5) == 3);
/// 

namespace fit {

namespace detail {

template<class T>
struct perfect_ref
{
    typedef T&& type;
    T&& value;
    constexpr perfect_ref(T&& x) : value(fit::forward<T>(x))
    {}
};

template<class T>
constexpr perfect_ref<T> make_perfect_ref(T&& x)
{
    return { fit::forward<T>(x) };
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
    FIT_RETURNS(fit::forward<typename T::type>(x.value));
};

template<int... N>
constexpr args_at<N...> make_args_at(seq<N...>)
{
    return {};
}

template<int N, class... Ts>
constexpr auto get_args(Ts&&... xs) FIT_RETURNS
(
    make_args_at(typename gens<N>::type())(nullptr, make_perfect_ref(fit::forward<Ts>(xs))...)
);

template<class T, T N>
struct args_f
{
    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) FIT_RETURNS
    (
        get_args<N>(fit::forward<Ts>(xs)...)
    );
};

}

// Deprecate
template<int N, class... Ts>
constexpr auto args(Ts&&... xs) FIT_RETURNS
(
    detail::get_args<N>(fit::forward<Ts>(xs)...)
);

template<class IntegralConstant>
constexpr detail::args_f<int, IntegralConstant::value> args(IntegralConstant)
{
    return detail::args_f<int, IntegralConstant::value>();
}

}

#endif
