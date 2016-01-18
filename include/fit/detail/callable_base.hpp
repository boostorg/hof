/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    callable_base.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_CALLABLE_BASE_H
#define FIT_GUARD_CALLABLE_BASE_H

#include <fit/detail/delegate.hpp>
#include <fit/detail/result_of.hpp>
#include <fit/apply.hpp>

#ifndef FIT_HAS_TEMPLATE_ALIAS
#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7)
#define FIT_HAS_TEMPLATE_ALIAS 0
#else
#define FIT_HAS_TEMPLATE_ALIAS 1
#endif
#endif

namespace fit { namespace detail {

template<class F>
struct non_class_function
{
    F f;
    FIT_DELGATE_CONSTRUCTOR(non_class_function, F, f)

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(apply_f, id_<F>, id_<Ts>...) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        fit::apply(f, fit::forward<Ts>(xs)...)
    );
};

#if FIT_HAS_TEMPLATE_ALIAS
template<class F>
using callable_base = typename std::conditional<(std::is_class<F>::value), F, non_class_function<F>>::type;
#else

template<class F>
struct callable_base_type
: std::conditional<(std::is_class<F>::value), F, non_class_function<F>>
{};

template<class F>
struct callable_base
: callable_base_type<F>::type
{
    typedef typename callable_base_type<F>::type base;
    FIT_INHERIT_CONSTRUCTOR(callable_base, base)
};

template<class F>
struct callable_base<callable_base<F>>
: callable_base<F>
{
    typedef callable_base<F> base;
    FIT_INHERIT_CONSTRUCTOR(callable_base, base)
};

#endif

}} // namespace fit

#endif
