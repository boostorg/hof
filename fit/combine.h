/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    combine.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_COMBINE_H
#define FIT_GUARD_COMBINE_H

#include <fit/pack.h>
#include <fit/always.h>
#include <fit/detail/result_of.h>
#include <fit/detail/make.h>

namespace fit { namespace detail {

template<class S, class F, class... Gs>
struct combine_adaptor_base;

template<int... Ns, class F, class... Gs>
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

// Result needs to be calulated in a seperate class to avoid confusing the
// compiler on MSVC
#if FIT_NO_EXPRESSION_SFINAE 
    template<class... Ts>
    struct combine_result
    : result_of<const F&,  result_of<const Gs&, id_<Ts>>...>
    {};
#endif

    template<class... Ts>
#if FIT_NO_EXPRESSION_SFINAE 
    constexpr typename combine_result<Ts...>::type
#else
    constexpr auto
#endif
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        (FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...)))
            (pack_get<Ns, Gs, pack_tag<Gs...>>(*FIT_CONST_THIS, xs)(fit::forward<Ts>(xs))...)
    );
};

}

template<class F, class... Gs>
struct combine_adaptor
: detail::combine_adaptor_base<typename detail::gens<sizeof...(Gs)>::type, F, Gs...>
{
    typedef detail::combine_adaptor_base<typename detail::gens<sizeof...(Gs)>::type, F, Gs...> base_type;
    FIT_INHERIT_CONSTRUCTOR(combine_adaptor, base_type)
};

FIT_DECLARE_STATIC_VAR(combine, detail::make<combine_adaptor>);

}

#endif
