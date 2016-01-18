/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    holder.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_HOLDER_H
#define FIT_GUARD_HOLDER_H

namespace fit { namespace detail {

template<class... Ts>
struct holder
{
    typedef void type;
};

}} // namespace fit

#endif
