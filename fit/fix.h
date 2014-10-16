/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    fix.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_FIX_H
#define FIT_GUARD_FUNCTION_FIX_H

/// fix
/// ===
/// 
/// Description
/// -----------
/// 
/// The `fix` function adaptor implements a fixed-point combinator. This can be
/// used to write recursive functions. 
/// 
/// Note: Compilers are too eager to instantiate templates when using
/// constexpr, which causes the compiler to reach its internal instantiation
/// limit. So, unfortunately, `fix` cannot be used for `constexpr` functions.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr fix_adaptor<F> fix(F f);
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
///     FunctionObject
///     MoveConstructible
/// 
/// Example
/// -------
/// 
///     int r = fit::fix([](auto s, auto x) -> decltype(x) { return x == 0 ? 1 : x * s(x-1); })(5);
///     assert(r == 5*4*3*2*1);
/// 

#include <fit/always.h>
#include <fit/returns.h>
#include <fit/detail/delegate.h>

#ifndef FIT_FIX_HAS_CONSTEXPR
#define FIT_FIX_HAS_CONSTEXPR 0
#endif

#if FIT_FIX_HAS_CONSTEXPR
#define FIT_FIX_CONSTEXPR constexpr
#else
#define FIT_FIX_CONSTEXPR
#endif


namespace fit {

namespace detail{

template<class Derived, class F>
struct fix_adaptor_base : F
{
    FIT_INHERIT_CONSTRUCTOR(fix_adaptor_base, F);

    template<class... Ts>
    FIT_FIX_CONSTEXPR const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Ts>
    FIT_FIX_CONSTEXPR const Derived& derived_function(Ts&&... xs) const
    {
        return static_cast<const Derived&>(always_ref(*this)(xs...));
    }

    FIT_RETURNS_CLASS(fix_adaptor_base);

    template<class... Ts>
    FIT_FIX_CONSTEXPR auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...))
            (FIT_MANGLE_CAST(const Derived&)(FIT_CONST_THIS->derived_function(xs...)), std::forward<Ts>(xs)...)
    );
};
}

template<class F>
struct fix_adaptor : detail::fix_adaptor_base<fix_adaptor<F>, F>
{
    typedef detail::fix_adaptor_base<fix_adaptor<F>, F> base;
    FIT_INHERIT_CONSTRUCTOR(fix_adaptor, base);
};

template<class F>
constexpr fix_adaptor<F> fix(F f)
{
    return fix_adaptor<F>(std::move(f));
}
}

#endif
