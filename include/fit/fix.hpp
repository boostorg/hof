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
/// When using `constexpr`, a function can recurse to a depth that is defined by
/// `FIT_RECURSIVE_CONSTEXPR_DEPTH`(default is 16). There is no limitiation on
/// recursion depth for non-constexpr functions. In addition, due to the
/// eagerness of `constexpr` to instantiation templates, in some cases, an
/// explicit return type must be specified in order to avoid reaching the
/// recursion limits of the compiler. This can be accomplished using
/// [`fit::result`](/include/fit/result):
/// 
///     int r = fit::result<int>(factorial)(5);
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
///         auto factorial = fit::fix(
///             [](auto recurse, auto x) -> decltype(x) { 
///                 return x == 0 ? 1 : x * recurse(x-1); 
///             }
///         );
///         int r = fit::result<int>(factorial)(5);
///         assert(r == 5*4*3*2*1);
///     }
/// 
/// References
/// ----------
/// 
/// * [Fixed-point combinator](https://en.wikipedia.org/wiki/Fixed-point_combinator)
/// * [Recursive](Recursive)
/// 

#include <fit/always.hpp>
#include <fit/detail/callable_base.hpp>
#include <fit/reveal.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/using.hpp>
#include <fit/detail/static_const_var.hpp>
#include <fit/indirect.hpp>
#include <fit/result.hpp>
#include <fit/detail/recursive_constexpr_depth.hpp>


namespace fit {

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
    template<class>
    struct result
    : fix_result<F>
    {};
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
    template<class>
    FIT_USING(result, fix_result<F>);

    template<class...>
    struct apply
    { 
        typedef typename F::result_type type; 
    };  
};

struct make_fix_result
{
    template<class F>
    FIT_USING(result, fix_result<F>);
};

template<int N, class Result=make_fix_result>
struct fix_adaptor_builder
{
    template<class F>
    struct base
    {
        // struct fix_failure
        // {
        //     template<class Failure>
        //     struct apply
        //     {
        //         template<class... Ts>
        //         struct of
        //         : Failure::template of<Ts...>
        //         {};
        //     };
        // };

        // struct failure
        // : failure_map<fix_failure, F>
        // {};
    };

    struct apply
    {
        template<class F, class... Ts, 
            class Adaptor=detail::unary_adaptor_builder<
                detail::fix_adaptor_builder<N-1, typename Result::template result<typename bare<F>::type>>
            >
        >
        constexpr FIT_SFINAE_RESULT(F&&, id_<Ts>...) 
        operator()(F&& f, Ts&&... xs) const FIT_SFINAE_RETURNS
        (
            FIT_FORWARD(F)(f)(
                fit::detail::make_adaptor<Adaptor>(fit::detail::make_indirect_ref(f)), 
                FIT_FORWARD(Ts)(xs)...
            )
        );
    };
};

template<class Result>
struct fix_adaptor_builder<0, Result>
{
    template<class F>
    struct base
    {
        // struct fix_failure
        // {
        //     template<class Failure>
        //     struct apply
        //     {
        //         template<class... Ts>
        //         struct of
        //         : Failure::template of<Ts...>
        //         {};
        //     };
        // };

        // struct failure
        // : failure_map<fix_failure, F>
        // {};
    };

    struct apply
    {
        typedef detail::unary_adaptor_builder<detail::fix_adaptor_builder<0, Result>> adaptor;

        template<class F, class... Ts>
        typename Result::template apply<decltype(fit::detail::make_adaptor<adaptor>(fit::detail::make_indirect_ref(std::declval<F>()))), Ts...>::type
        operator()(F&& f, Ts&&... xs) const
        {
            return FIT_FORWARD(F)(f)(
                fit::detail::make_adaptor<adaptor>(fit::detail::make_indirect_ref(f)), 
                FIT_FORWARD(Ts)(xs)...
            );
        }
    };
};
}

FIT_DECLARE_ADAPTOR(fix, detail::unary_adaptor_builder<detail::fix_adaptor_builder<FIT_RECURSIVE_CONSTEXPR_DEPTH>>)

} // namespace fit

#endif
