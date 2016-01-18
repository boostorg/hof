/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    indirect.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_INDIRECT_H
#define FIT_GUARD_FUNCTION_INDIRECT_H

/// indirect
/// ========
/// 
/// Description
/// -----------
/// 
/// The `indirect` function adaptor dereferences the object before calling it.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr indirect_adaptor<F> indirect(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(indirect(f)(xs...) == (*f)(xs...));
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [FunctionObject](concepts.md#functionobject)
/// * MoveConstructible
/// * Dereferenceable
/// 
/// Example
/// -------
/// 
///     struct sum
///     {
///         template<class T, class U>
///         T operator()(T x, U y) const
///         {
///             return x+y;
///         }
///     };
/// 
///     int r = indirect(std::make_unique<sum>())(3,2);
///     assert(r == 5);
/// 

#include <fit/detail/delegate.hpp>
#include <fit/detail/result_of.hpp>
#include <fit/reveal.hpp>
#include <fit/always.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/static_const_var.hpp>

namespace fit {
// TODO: Support non-classes as well
template<class F>
struct indirect_adaptor : F
{
    typedef indirect_adaptor fit_rewritable1_tag;
    FIT_INHERIT_CONSTRUCTOR(indirect_adaptor, F);

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    struct failure
    : failure_for<decltype(*std::declval<F>())>
    {};

    FIT_RETURNS_CLASS(indirect_adaptor);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(decltype(*std::declval<F>()), id_<Ts>...) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        (*FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...)))(fit::forward<Ts>(xs)...)
    );
};

FIT_DECLARE_STATIC_VAR(indirect, detail::make<indirect_adaptor>);

} // namespace fit

#endif
