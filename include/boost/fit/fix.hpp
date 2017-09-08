/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    fix.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FUNCTION_FIX_H
#define BOOST_FIT_GUARD_FUNCTION_FIX_H

/// fix
/// ===
/// 
/// Description
/// -----------
/// 
/// The `fix` function adaptor implements a fixed-point combinator. This can be
/// used to write recursive functions. 
/// 
/// When using `constexpr`, a function can recurse to a depth that is defined by
/// `BOOST_FIT_RECURSIVE_CONSTEXPR_DEPTH`(default is 16). There is no limitiation on
/// recursion depth for non-constexpr functions. In addition, due to the
/// eagerness of `constexpr` to instantiation templates, in some cases, an
/// explicit return type must be specified in order to avoid reaching the
/// recursion limits of the compiler. This can be accomplished using
/// [`boost::fit::result`](/include/boost/fit/result):
/// 
///     int r = boost::fit::result<int>(factorial)(5);
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
///     assert(fix(f)(xs...) == f(fix(f), xs...));
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [ConstFunctionObject](ConstFunctionObject)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
/// 
///     int main() {
///         auto factorial = boost::fit::fix(
///             [](auto recurse, auto x) -> decltype(x) { 
///                 return x == 0 ? 1 : x * recurse(x-1); 
///             }
///         );
///         int r = boost::fit::result<int>(factorial)(5);
///         assert(r == 5*4*3*2*1);
///     }
/// 
/// References
/// ----------
/// 
/// * [Fixed-point combinator](https://en.wikipedia.org/wiki/Fixed-point_combinator)
/// * [Recursive](Recursive)
/// 

#include <boost/fit/always.hpp>
#include <boost/fit/detail/callable_base.hpp>
#include <boost/fit/reveal.hpp>
#include <boost/fit/detail/delegate.hpp>
#include <boost/fit/detail/move.hpp>
#include <boost/fit/detail/make.hpp>
#include <boost/fit/detail/static_const_var.hpp>
#include <boost/fit/indirect.hpp>
#include <boost/fit/result.hpp>
#include <boost/fit/detail/recursive_constexpr_depth.hpp>


namespace boost { namespace fit {

namespace detail{

template<class F>
struct compute_indirect_ref
{ typedef indirect_adaptor<const F*> type; };

template<class F>
struct compute_indirect_ref<indirect_adaptor<F*>>
{ typedef indirect_adaptor<F*> type; };

template<class F>
constexpr indirect_adaptor<const F*> make_indirect_ref(const F& f) noexcept
{
    return indirect_adaptor<const F*>(&f);
}

template<class F>
constexpr indirect_adaptor<const F*> make_indirect_ref(const indirect_adaptor<F*>& f) noexcept
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
    BOOST_FIT_INHERIT_CONSTRUCTOR(fix_adaptor_base, F);

    typedef typename compute_indirect_ref<F>::type base_ref_type;
    typedef fix_adaptor_base<base_ref_type, Result, N-1> derived;


    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const noexcept
    {
        return boost::fit::always_ref(*this)(xs...);
    }

    template<class... Ts>
    constexpr derived derived_function(Ts&&... xs) const noexcept
    {
        return derived(boost::fit::detail::make_indirect_ref(this->base_function(xs...)));
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


    BOOST_FIT_RETURNS_CLASS(fix_adaptor_base);

    template<class... Ts>
    constexpr BOOST_FIT_SFINAE_RESULT(const F&, id_<derived>, id_<Ts>...) 
    operator()(Ts&&... xs) const BOOST_FIT_SFINAE_RETURNS
    (
        BOOST_FIT_MANGLE_CAST(const F&)(BOOST_FIT_CONST_THIS->base_function(xs...))
            (
                BOOST_FIT_MANGLE_CAST(derived)(BOOST_FIT_CONST_THIS->derived_function(xs...)), 
                BOOST_FIT_FORWARD(Ts)(xs)...
            )
    );
};

template<class F, class Result>
struct fix_adaptor_base<F, Result, 0> : F
{
    BOOST_FIT_INHERIT_CONSTRUCTOR(fix_adaptor_base, F);

    template<class... Ts>
    const F& base_function(Ts&&...) const noexcept
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


    BOOST_FIT_RETURNS_CLASS(fix_adaptor_base);

    template<class... Ts>
    typename Result::template apply<fix_adaptor_base, Ts...>::type
    operator()(Ts&&... xs) const
    {
        return this->base_function(xs...)(*this, BOOST_FIT_FORWARD(Ts)(xs)...);
    }
};
}

template<class F>
struct fix_adaptor : detail::fix_adaptor_base<F, detail::fix_result<F>, BOOST_FIT_RECURSIVE_CONSTEXPR_DEPTH>
{
    typedef fix_adaptor fit_rewritable1_tag;
    typedef detail::fix_adaptor_base<F, detail::fix_result<F>, BOOST_FIT_RECURSIVE_CONSTEXPR_DEPTH> base;
    BOOST_FIT_INHERIT_CONSTRUCTOR(fix_adaptor, base);
};

template<class Result, class F>
struct result_adaptor<Result, fix_adaptor<F>>
: fix_adaptor<result_adaptor<Result, F>>
{
    typedef fix_adaptor<result_adaptor<Result, F>> base;
    BOOST_FIT_INHERIT_CONSTRUCTOR(result_adaptor, base)
};

BOOST_FIT_DECLARE_STATIC_VAR(fix, detail::make<fix_adaptor>);

}} // namespace boost::fit

#endif
