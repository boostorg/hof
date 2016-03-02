/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    ref_tuple.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FUNCTION_REF_TUPLE_H
#define BOOST_FIT_GUARD_FUNCTION_REF_TUPLE_H

#include <tuple>
#include <boost/fit/detail/remove_rvalue_reference.hpp>
#include <boost/fit/returns.hpp>

namespace boost { namespace fit {
namespace detail {

template<class... Ts>
constexpr auto make_ref_tuple(Ts&&... x) BOOST_FIT_RETURNS
(
    std::tuple<typename remove_rvalue_reference<Ts>::type...>(boost::fit::forward<Ts>(x)...)
);
}
}} // namespace boost::fit

#endif
