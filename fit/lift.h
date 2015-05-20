/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    lift.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_LIFT_H
#define FIT_GUARD_FUNCTION_LIFT_H

#include <fit/returns.h>
#include <fit/detail/forward.h>

#define FIT_LIFT(...) [](auto&&... xs) FIT_RETURNS(__VA_ARGS__(fit::forward<decltype(xs)>(xs)...))

#define FIT_LIFT_CLASS(name, ...) \
struct name \
{ \
    template<class... Ts> \
    constexpr auto operator()(Ts&&... xs) const \
    FIT_RETURNS(__VA_ARGS__(fit::forward<Ts>(xs)...)) \
}

#endif
