/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    combine.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_COMBINE_H
#define FIT_GUARD_COMBINE_H

/// combine
/// =======
/// 
/// Description
/// -----------
/// 
/// The `combine` function adaptor combines several functions together with
/// their arguments. It essentially zips each function with an argument before
/// calling the main function.
/// 
/// Synopsis
/// --------
/// 
///     template<class F, class... Gs>
///     constexpr combine_adaptor<F, Gs...> combine(F f, Gs... gs);
/// 
/// Semantics
/// ---------
/// 
///     assert(combine(f, gs...)(xs...) == f(gs(xs)...));
/// 
/// Requirements
/// ------------
/// 
/// F and Gs must be:
/// 
/// * [Callable](concepts.md#callable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     auto f = fit::combine(
///         fit::construct<std::tuple>(),
///         fit::capture(1)(fit::construct<std::pair>()),
///         fit::capture(2)(fit::construct<std::pair>()));
///     assert(f(2, 4) == std::make_tuple(std::make_pair(1, 2), std::make_pair(2, 4)));
/// 

#include <fit/pack.hpp>
#include <fit/always.hpp>
#include <fit/detail/callable_base.hpp>
#include <fit/detail/make.hpp>

namespace fit { namespace detail {

template<class S, class F, class... Gs>
struct combine_adaptor_base;

template<std::size_t... Ns, class F, class... Gs>
struct combine_adaptor_base<seq<Ns...>, F, Gs...>
: F, pack_base<seq<Ns...>, Gs...>
{
    typedef pack_base<seq<Ns...>, Gs...> base_type;

    FIT_INHERIT_DEFAULT(combine_adaptor_base, base_type, F)

    template<class X, class... Xs, 
        FIT_ENABLE_IF_CONSTRUCTIBLE(F, X),
        FIT_ENABLE_IF_CONSTRUCTIBLE(base_type, Xs...)>
    constexpr combine_adaptor_base(X&& x, Xs&&... xs) 
    : F(fit::forward<X>(x)), base_type(fit::forward<Xs>(xs)...)
    {}

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(combine_adaptor_base);

// Result needs to be calculated in a separate class to avoid confusing the
// compiler on MSVC
#if FIT_NO_EXPRESSION_SFINAE || FIT_HAS_MANUAL_DEDUCTION
    template<class... Ts>
    struct combine_result
    : result_of<const F&,  result_of<const Gs&, id_<Ts>>...>
    {};
#endif

    template<class... Ts>
#if FIT_NO_EXPRESSION_SFINAE || FIT_HAS_MANUAL_DEDUCTION
    constexpr typename combine_result<Ts...>::type
#else
    constexpr auto
#endif
    operator()(Ts&&... xs) const FIT_SFINAE_MANUAL_RETURNS
    (
        (FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...)))
            (alias_value<pack_tag<seq<Ns>, Gs...>, Gs>(*FIT_CONST_THIS, xs)(fit::forward<Ts>(xs))...)
    );
};

}

template<class F, class... Gs>
struct combine_adaptor
: detail::combine_adaptor_base<typename detail::gens<sizeof...(Gs)>::type, detail::callable_base<F>, detail::callable_base<Gs>...>
{
    typedef detail::combine_adaptor_base<typename detail::gens<sizeof...(Gs)>::type, detail::callable_base<F>, detail::callable_base<Gs>...> base_type;
    FIT_INHERIT_CONSTRUCTOR(combine_adaptor, base_type)
};

FIT_DECLARE_STATIC_VAR(combine, detail::make<combine_adaptor>);

} // namespace fit

#endif
