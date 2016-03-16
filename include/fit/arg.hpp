/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    arg.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_ARGS_H
#define FIT_GUARD_FUNCTION_ARGS_H

#include <fit/detail/seq.hpp>
#include <fit/returns.hpp>
#include <fit/detail/static_const_var.hpp>
#include <utility>

/// arg
/// ===
/// 
/// Description
/// -----------
/// 
/// The `arg` function returns a function object that returns the Nth argument
/// passed to it. It actually starts at 1, so it is not the zero-based index
/// of the argument.
/// 
/// Synopsis
/// --------
/// 
///     template<class IntegralConstant>
///     constexpr auto arg(IntegralConstant);
/// 
///     template<std::size_t N, class... Ts>
///     constexpr auto arg_c(Ts&&...);
/// 
/// 
/// Example
/// -------
/// 
///     assert(arg(std::integral_constant<int, 3>())(1,2,3,4,5) == 3);
/// 

namespace fit {

namespace detail {

template<class T>
struct perfect_ref
{
    typedef T&& type;
    T&& value;
    constexpr perfect_ref(T&& x) : value(FIT_FORWARD(T)(x))
    {}
};

template<class T>
constexpr perfect_ref<T> make_perfect_ref(T&& x)
{
    return { FIT_FORWARD(T)(x) };
}

template<std::size_t N>
struct ignore
{
    template<class T>
    constexpr ignore(T&&...)
    {}
};

template<std::size_t... N>
struct args_at
{
    template<class T, class... Ts>
    constexpr auto operator()(ignore<N>..., T x, Ts...) const 
    FIT_RETURNS(FIT_FORWARD(typename T::type)(x.value));
};

template<std::size_t... N>
constexpr args_at<N...> make_args_at(seq<N...>)
{
    return {};
}

template<std::size_t N, class... Ts>
constexpr auto get_args(Ts&&... xs) FIT_RETURNS
(
    make_args_at(typename gens<N>::type())(nullptr, make_perfect_ref(FIT_FORWARD(Ts)(xs))...)
);

template<class T, T N>
struct make_args_f
{
    template<class... Ts, class=typename std::enable_if<(N <= sizeof...(Ts))>::type>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        get_args<N>(FIT_FORWARD(Ts)(xs)...)
    );
};

struct arg_f
{
    template<class IntegralConstant>
    constexpr make_args_f<std::size_t, IntegralConstant::value> operator()(IntegralConstant) const
    {
        return make_args_f<std::size_t, IntegralConstant::value>();
    }
};

}

template<std::size_t N, class... Ts>
constexpr auto arg_c(Ts&&... xs) FIT_RETURNS
(
    detail::get_args<N>(FIT_FORWARD(Ts)(xs)...)
);

FIT_DECLARE_STATIC_VAR(arg, detail::arg_f);

} // namespace fit

#endif
