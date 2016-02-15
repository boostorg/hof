/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    cast.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_DETAIL_CAST_HPP
#define FIT_GUARD_DETAIL_CAST_HPP

namespace fit { namespace detail {

template<class Base, class T, class... Ts>
constexpr const Base& cast(const T& x, Ts&&...)
{
    return x;
}


}

} // namespace fit

#endif
