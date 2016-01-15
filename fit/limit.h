/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    limit.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_LIMIT_H
#define FIT_GUARD_LIMIT_H

#include <fit/detail/callable_base.h>
#include <fit/detail/forward.h>
#include <fit/detail/delegate.h>
#include <fit/detail/move.h>
#include <fit/detail/static_const_var.h>
#include <cstdint>

namespace fit {

namespace detail {

template<std::size_t N, class F>
struct limit_adaptor : detail::callable_base<F>
{
    typedef std::integral_constant<std::size_t, N> fit_function_param_limit;
    FIT_INHERIT_CONSTRUCTOR(limit_adaptor, detail::callable_base<F>)
};

template<std::size_t N>
struct make_limit_f
{
    constexpr make_limit_f()
    {}
    template<class F>
    constexpr limit_adaptor<N, F> operator()(F f) const
    {
        return limit_adaptor<N, F>(fit::move(f));
    }
};

struct limit_f
{
    template<class IntegralConstant, std::size_t N=IntegralConstant::type::value>
    constexpr make_limit_f<N> operator()(IntegralConstant) const
    {
        return {};
    }
};

}

template<std::size_t N, class F>
constexpr detail::limit_adaptor<N, F> limit_c(F f)
{
    return detail::limit_adaptor<N, F>(fit::move(f));
}

FIT_DECLARE_STATIC_VAR(limit, detail::limit_f);


template<class F, class=void>
struct function_param_limit
: std::integral_constant<std::size_t, SIZE_MAX>
{};

template<class F>
struct function_param_limit<F, typename detail::holder<typename F::fit_function_param_limit>::type>
: F::fit_function_param_limit
{};

}

#endif