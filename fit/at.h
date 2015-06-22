/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    at.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_AT_H
#define FIT_GUARD_FUNCTION_AT_H

#include <fit/detail/seq.h>
#include <fit/returns.h>
#include <fit/detail/static_const_var.h>
#include <utility>

/// at
/// ====
/// 
/// Description
/// -----------
/// 
/// The `at` returns a function object that returns the Nth argument passed
/// to it.
/// 
/// Synopsis
/// --------
/// 
///     template<class IntegralConstant>
///     constexpr auto at(IntegralConstant);
/// 
/// 
/// Example
/// -------
/// 
///     assert(at(std::integral_constant<int, 3>())(0,1,2,3,4,5) == 3);
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
struct at_impl
{
    template<class T, class... Ts>
    constexpr auto operator()(ignore<N>..., T x, Ts...) const 
    FIT_RETURNS(fit::forward<typename T::type>(x.value));
};

template<int... N>
constexpr at_impl<N...> make_at_impl(seq<N...>)
{
    return {};
}

template<int N, class... Ts>
constexpr auto get_at(Ts&&... xs) FIT_RETURNS
(
    make_at_impl(typename gens<N>::type())(make_perfect_ref(fit::forward<Ts>(xs))...)
);

template<class T, T N>
struct at_adaptor
{
    template<class... Ts, class=typename std::enable_if<(
        N >=0 && N < sizeof...(Ts)
    )>::type>
    constexpr auto operator()(Ts&&... xs) FIT_RETURNS
    (
        get_at<N>(fit::forward<Ts>(xs)...)
    );
};

struct at_f
{
    template<class IntegralConstant>
    constexpr detail::at_adaptor<int, IntegralConstant::value> operator()(IntegralConstant) const
    {
        return detail::at_adaptor<int, IntegralConstant::value>();
    }
};

}

FIT_DECLARE_STATIC_VAR(at, detail::at_f);

}

#endif
