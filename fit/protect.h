/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    protect.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_PROTECT_H
#define FIT_GUARD_FUNCTION_PROTECT_H

#include <utility>

namespace fit {

template<class F>
struct protect_adaptor : F
{
    template<class... Ts>
    constexpr protect_adaptor(Ts&&... xs) : F(std::forward<Ts>(xs)...)
    {}
};

template<class F>
protect_adaptor<F> protect(F f)
{
    return protect_adaptor<F>(f);
}

}
#endif
