/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    mutable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_MUTABLE_H
#define FIT_GUARD_FUNCTION_MUTABLE_H

/// mutable
/// =======
/// 
/// Description
/// -----------
/// 
/// The `mutable` function adaptor allows using a non-const function object
/// inside of a const-function object. In Fit, all the function adaptors use
/// `const` call overloads, so if there is a function that has a non-const
/// call operator, it couldn't be used directly. So, `mutable_` allows the
/// function to be used inside of the call operator.
/// 
/// NOTE: This function should be used with caution since many functions are
/// copied, so relying on some internal shared state can be error-prone.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     mutable_adaptor<F> mutable_(F f)
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
///     MutableFunctionObject
///     MoveConstructible
/// 

#include <fit/returns.h>
#include <fit/detail/delegate.h>

namespace fit {

template<class F>
struct mutable_adaptor
{
    mutable F f;

    FIT_DELGATE_CONSTRUCTOR(mutable_adaptor, F, f);

    FIT_RETURNS_CLASS(mutable_adaptor);

    template<class... Ts>
    auto operator()(Ts&&... xs) const FIT_RETURNS(FIT_CONST_THIS->f(std::forward<Ts>(xs)...));
};

template<class F>
mutable_adaptor<F> mutable_(F f)
{
    return mutable_adaptor<F>(std::move(f));
}

}


#endif
