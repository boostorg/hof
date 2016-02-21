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
/// Semantics
/// ---------
/// 
///     assert(fix(f)(xs...) == f(f, xs...));
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [Callable](concepts.md#callable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     int r = fit::fix([](auto recurse, auto x) -> decltype(x) { return x == 0 ? 1 : x * recurse(x-1); })(5);
///     assert(r == 5*4*3*2*1);
/// 

#include <fit/always.hpp>
#include <fit/detail/callable_base.hpp>
#include <fit/reveal.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/static_const_var.hpp>
#include <fit/indirect.hpp>
#include <fit/result.hpp>


namespace fit {

namespace detail{

template<class F>
struct compute_indirect_ref
{ typedef indirect_adaptor<const F*> type; };

template<class F>
struct compute_indirect_ref<indirect_adaptor<F*>>
{ typedef indirect_adaptor<F*> type; };

template<class F>
constexpr indirect_adaptor<const F*> make_indirect_ref(const F& f)
{
    return indirect_adaptor<const F*>(&f);
}

template<class F>
constexpr indirect_adaptor<const F*> make_indirect_ref(const indirect_adaptor<F*>& f)
{
    return f;
}

template<class F, class=void>
struct fix_result
{
    template<class... Ts>
    struct apply
    { 
        typedef decltype(std::declval<F>()(std::declval<Ts>()...)) type; 
    };
};

template<class F>
struct fix_result<F, typename holder<
    typename F::result_type
>::type>
{
    template<class...>
    struct apply
    { 
        typedef typename F::result_type type; 
    };
    
};

template<class F, class Result, int N>
struct fix_adaptor_base : F
{
    FIT_INHERIT_CONSTRUCTOR(fix_adaptor_base, F);

    typedef typename compute_indirect_ref<F>::type base_ref_type;
    typedef fix_adaptor_base<base_ref_type, Result, N-1> derived;


    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Ts>
    constexpr derived derived_function(Ts&&... xs) const
    {
        return derived(make_indirect_ref(this->base_function(xs...)));
    }

    struct fix_failure
    {
        template<class Failure>
        struct apply
        {
            template<class... Ts>
            struct of
            : Failure::template of<derived, Ts...>
            {};
        };
    };

    struct failure
    : failure_map<fix_failure, F>
    {};


    FIT_RETURNS_CLASS(fix_adaptor_base);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(const F&, id_<derived>, id_<Ts>...) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...))
            (
                FIT_MANGLE_CAST(derived)(FIT_CONST_THIS->derived_function(xs...)), 
                FIT_FORWARD(Ts)(xs)...
            )
    );
};

template<class F, class Result>
struct fix_adaptor_base<F, Result, 0> : F
{
    FIT_INHERIT_CONSTRUCTOR(fix_adaptor_base, F);

    template<class... Ts>
    const F& base_function(Ts&&...) const
    {
        return *this;
    }

    struct fix_failure
    {
        template<class Failure>
        struct apply
        {
            template<class... Ts>
            struct of
            : Failure::template of<fix_adaptor_base, Ts...>
            {};
        };
    };

    struct failure
    : failure_map<fix_failure, F>
    {};


    FIT_RETURNS_CLASS(fix_adaptor_base);

    template<class... Ts>
    typename Result::template apply<fix_adaptor_base, Ts...>::type
    operator()(Ts&&... xs) const 
    {
        return this->base_function(xs...)(*this, FIT_FORWARD(Ts)(xs)...);
    }
};
}

template<class F>
struct fix_adaptor : detail::fix_adaptor_base<F, detail::fix_result<F>, 32>
{
    typedef fix_adaptor fit_rewritable1_tag;
    typedef detail::fix_adaptor_base<F, detail::fix_result<F>, 32> base;
    FIT_INHERIT_CONSTRUCTOR(fix_adaptor, base);
};

template<class Result, class F>
struct result_adaptor<Result, fix_adaptor<F>>
: fix_adaptor<result_adaptor<Result, F>>
{
    typedef fix_adaptor<result_adaptor<Result, F>> base;
    FIT_INHERIT_CONSTRUCTOR(result_adaptor, base)
};

FIT_DECLARE_STATIC_VAR(fix, detail::make<fix_adaptor>);

} // namespace fit

#endif
