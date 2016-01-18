/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    seq.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_DETAIL_SEQ_H
#define FIT_GUARD_FUNCTION_DETAIL_SEQ_H


namespace fit { 

namespace detail {

template<int ...>
struct seq {};

template<int N, int ...S>
struct gens : gens<N-1, N-1, S...> {};

template<int ...S>
struct gens<0, S...> 
{
  typedef seq<S...> type;
};

}
}

#endif