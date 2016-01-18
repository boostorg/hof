/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    static.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_STATIC_H
#define FIT_GUARD_FUNCTION_STATIC_H

/// static
/// ======
/// 
/// Description
/// -----------
/// 
/// The `static_` adaptor is a static function adaptor that allows any
/// default-constructible function object to be static-initialized. Functions
/// initialized by `static_` cannot be used in `constexpr` functions. If the
/// function needs to be statically initialized and called in a `constexpr`
/// context, then a `constexpr` constructor needs to be used rather than
/// `static_`.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     class static_;
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [FunctionObject](concepts.md#functionobject)
/// * DefaultConstructible
/// 
/// Example
/// -------
/// 
///     // In C++ this class can't be static-initialized, because of the non-
///     // trivial default constructor.
///     struct times_function
///     {
///         double factor;
///         times2_function() : factor(2)
///         {}
///         template<class T>
///         T operator()(T x) const
///         {
///             return x*factor;
///         }
///     };
/// 
///     static_<times_function> times2 = {};
/// 
///     assert(6 == times2(3));
/// 

#include <fit/detail/result_of.hpp>
#include <fit/reveal.hpp>

namespace fit { 

template<class F>
struct static_
{

    struct failure
    : failure_for<F>
    {};

    const F& base_function() const
    {
        static F f;
        return f;
    }

    FIT_RETURNS_CLASS(static_);

    template<class... Ts>
    FIT_SFINAE_RESULT(F, id_<Ts>...) 
    operator()(Ts && ... xs) const
    FIT_SFINAE_RETURNS(FIT_CONST_THIS->base_function()(fit::forward<Ts>(xs)...));
};


} // namespace fit

#endif
