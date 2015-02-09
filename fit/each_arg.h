/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    each_arg.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_EACH_ARG_H
#define FIT_GUARD_EACH_ARG_H

#include <fit/detail/forward.h>
#include <initializer_list>

namespace fit {

namespace detail {

struct each_arg_f
{
    template <class F, class... Ts> 
    constexpr void operator()(F&& f, Ts&&... a) const
    { 
        (void)std::initializer_list<int>{(f(fit::forward<Ts>(a)), 0)...}; 
    }
};
}

const detail::each_arg_f each_arg = {};

}

#endif