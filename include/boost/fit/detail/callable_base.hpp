/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    callable_base.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_CALLABLE_BASE_H
#define BOOST_FIT_GUARD_CALLABLE_BASE_H

#include <boost/fit/detail/delegate.hpp>
#include <boost/fit/detail/result_of.hpp>
#include <boost/fit/apply.hpp>

#ifndef BOOST_FIT_HAS_TEMPLATE_ALIAS
#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7)
#define BOOST_FIT_HAS_TEMPLATE_ALIAS 0
#else
#define BOOST_FIT_HAS_TEMPLATE_ALIAS 1
#endif
#endif

namespace boost { namespace fit { namespace detail {

template<class F>
struct non_class_function
{
    F f;
    BOOST_FIT_DELGATE_CONSTRUCTOR(non_class_function, F, f)

    template<class... Ts>
    constexpr BOOST_FIT_SFINAE_RESULT(apply_f, id_<F>, id_<Ts>...) 
    operator()(Ts&&... xs) const BOOST_FIT_SFINAE_RETURNS
    (
        boost::fit::apply(f, BOOST_FIT_FORWARD(Ts)(xs)...)
    );
};

template<class F>
struct callable_base_type
: std::conditional<(BOOST_FIT_IS_CLASS(F)), F, non_class_function<F>>
{};
#if BOOST_FIT_HAS_TEMPLATE_ALIAS
template<class F>
using callable_base = typename callable_base_type<F>::type;
#else
template<class F>
struct callable_base
: callable_base_type<F>::type
{
    typedef typename callable_base_type<F>::type base;
    BOOST_FIT_INHERIT_CONSTRUCTOR(callable_base, base)
};

template<class F>
struct callable_base<callable_base<F>>
: callable_base<F>
{
    typedef callable_base<F> base;
    BOOST_FIT_INHERIT_CONSTRUCTOR(callable_base, base)
};

#endif

}}} // namespace boost::fit

#endif
