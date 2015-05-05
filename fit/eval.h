/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    eval.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_EVAL_H
#define FIT_GUARD_EVAL_H

#include <fit/always.h>
#include <fit/identity.h>
#include <fit/conditional.h>
#include <fit/returns.h>

namespace fit {

namespace detail {

struct simple_eval
{
    template<class F, class... Ts>
    constexpr auto operator()(F&& f, Ts&&...xs) const FIT_RETURNS
    (fit::always_ref(f)(xs...)());
};

struct id_eval
{
    template<class F, class... Ts>
    constexpr auto operator()(F&& f, Ts&&...xs) const FIT_RETURNS
    (fit::always_ref(f)(xs...)(fit::identity));
};

}

FIT_STATIC_CONSTEXPR fit::conditional_adaptor<detail::id_eval, detail::simple_eval> eval = {};

}

#endif
