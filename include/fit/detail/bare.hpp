/*=============================================================================
    Copyright (c) 2017 Paul Fultz II
    bare.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_DETAIL_BARE_HPP
#define FIT_GUARD_DETAIL_BARE_HPP

#include <type_traits>

namespace fit { namespace detail {

template<class T>
struct bare
: std::remove_cv<typename std::remove_reference<T>::type>
{};

}

} // namespace fit

#endif
