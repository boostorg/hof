/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    invoke.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_INVOKE_H
#define FIT_GUARD_FUNCTION_INVOKE_H

/// invoke
/// ======
/// 
/// Description
/// -----------
/// 
/// Calls a function object with the arguments from a tuple.
/// 
/// Synopsis
/// --------
/// 
///     template<class F, class Sequence>
///     auto invoke(F f, const Sequence& seq);
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
///     FunctionObject
///     MoveConstructible
/// 
/// Sequence must be a:
/// 
///     TupleSequence
/// 
/// Example
/// -------
/// 
///     std::plus<int> add;
///     assert(invoke(add,std::make_tuple(1,1)) == 2);
/// 

#include <fit/returns.h>
#include <tuple>
#include <fit/detail/seq.h>

namespace fit { 

//
// invoke
//
namespace detail {

template<class Sequence>
constexpr typename gens<std::tuple_size<Sequence>::value>::type 
make_sequence_gens(const Sequence&)
{
	return {};
}

template<class F, class T, int ...N>
constexpr auto invoke_impl(F&& f, T && t, seq<N...>) FIT_RETURNS
(
    f(FIT_AUTO_FORWARD(std::get<N>(t))...)
);

}

template<class F, class Sequence>
constexpr auto invoke(F&& f, Sequence && t) FIT_RETURNS
(
    detail::invoke_impl(std::forward<F>(f), std::forward<Sequence>(t), detail::make_sequence_gens(t))
);


}

#endif
