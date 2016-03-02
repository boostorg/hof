/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    seq.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FUNCTION_DETAIL_SEQ_H
#define BOOST_FIT_GUARD_FUNCTION_DETAIL_SEQ_H

#include <cstdlib>

namespace boost { namespace fit { 

namespace detail {

template<std::size_t ...>
struct seq {};

template<std::size_t N, std::size_t ...S>
struct gens : gens<N-1, N-1, S...> {};

template<std::size_t ...S>
struct gens<0, S...> 
{
  typedef seq<S...> type;
};

}
}} // namespace boost::fit

#endif
