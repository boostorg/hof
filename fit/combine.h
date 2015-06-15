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

#ifndef FIT_USE_COMBINE_FOLD
#define FIT_USE_COMBINE_FOLD 1
#endif

#if FIT_USE_COMBINE_FOLD
#include <fit/compress.h>
#include <fit/args.h>
#include <fit/capture.h>
#include <fit/unpack.h>
#endif

namespace fit { namespace detail {

#if FIT_USE_COMBINE_FOLD
struct combine_fold
{
    template<class Gp, class Tp, class State, class N>
    auto operator()(Gp&& gp, Tp&& tp, State&& s, N n) const
    {
        return pack_join(fit::forward<State>(s), pack(gp(args(n))(tp(args(n)))));
    }
};

#endif

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

    template<class... Ts>
    constexpr const base_type& get_pack(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(combine_adaptor_base);
  
    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(const F&, result_of<const Gs&, id_<Ts>>...) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
#if FIT_USE_COMBINE_FOLD
        fit::unpack(this->base_function(xs...))(
            fit::compress(
                fit::capture(this->get_pack(xs...), pack_forward(fit::forward<Ts>(xs)...))(detail::combine_fold()), pack()
            )(std::integral_constant<int, Ns+1>()...)
        )
#else
        (FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...)))
            (pack_get<Ns, Gs, pack_tag<Gs...>>(*FIT_CONST_THIS, xs)(fit::forward<Ts>(xs))...)
#endif
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
