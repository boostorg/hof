/*=============================================================================
    Copyright (c) 2017 Paul Fultz II
    fold.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_DETAIL_BUILDER_FOLD_HPP
#define FIT_GUARD_DETAIL_BUILDER_FOLD_HPP

#include <fit/detail/builder.hpp>
#include <fit/detail/builder/unary.hpp>
#include <fit/detail/compressed_pair.hpp>

// #include <fit/pack.hpp>
// #include <fit/compress.hpp>
// #include <fit/capture.hpp>

namespace fit { namespace detail {

template<class BinaryAdaptor>
struct fold_adaptor_builder
{
    template<class... Fs>
    struct apply;


    template<class F>
    struct apply<F>
    : F
    {
        typedef F base;
        typedef apply fit_rewritable1_tag;

        FIT_INHERIT_CONSTRUCTOR(apply, base)
    };

#if 0
    struct op
    {
        template<class F, class G>
        constexpr auto
        // FIT_SFINAE_MANUAL_RESULT(typename BinaryAdaptor::apply, id_<const F&>, id_<const G&>, id_<Ts>...) 
        operator()(F&& f, G&& g) const FIT_SFINAE_MANUAL_RETURNS
        (
            fit::capture_basic(FIT_FORWARD(F)(f), FIT_FORWARD(G)(g))
            (FIT_RETURNS_CONSTRUCT(typename BinaryAdaptor::apply)())
            
        );
    };

    // struct fold_op
    // {
    //     template<class F, class G>
    //     constexpr auto operator()(const F& f, const G& g) const FIT_RETURNS
    //     (

    //     );
    // };
    template<class F, class... Fs>
    struct apply<F, Fs...>
    : fit::detail::pack_base<typename detail::gens<sizeof...(Fs)+1>::type, F, Fs...>
    {
        typedef fit::detail::pack_base<typename detail::gens<sizeof...(Fs)+1>::type, F, Fs...> base;
        typedef apply fit_rewritable_tag;

        template<class... Ts>
        constexpr const base& base_pack(Ts&&... xs) const noexcept
        {
            return always_ref(*this)(xs...);
        }

        // struct unpacked
        // {
        //     template<class... Ts>
        //     constexpr 
        //     auto
        //     // FIT_SFINAE_MANUAL_RESULT(typename BinaryAdaptor::apply, id_<const F&>, id_<const G&>, id_<Ts>...) 
        //     operator()(const F& f, const Fs&... fs, Ts&&... xs) const FIT_RETURNS
        //     (
        //         fit::compress(fit::capture_forward(FIT_RETURNS_CONSTRUCT(op)()))
        //         (f, fs...)
        //         (FIT_FORWARD(Ts)(xs)...)
        //     );
        // };

        FIT_RETURNS_CLASS(apply);

        template<class... Ts>
        constexpr 
        auto
        // FIT_SFINAE_MANUAL_RESULT(typename BinaryAdaptor::apply, id_<const F&>, id_<const G&>, id_<Ts>...) 
        operator()(Ts&&... xs) const FIT_RETURNS
        (
            FIT_MANGLE_CAST(const base&)(FIT_CONST_THIS->base_pack(xs...))
            (fit::compress(FIT_RETURNS_CONSTRUCT(op)()))
            (FIT_FORWARD(Ts)(xs)...)
        );

        FIT_INHERIT_CONSTRUCTOR(apply, base)

    };
#else
    template<class F, class G>
    struct apply<F, G>
    : 
        detail::compressed_pair<F, G>, 
        BinaryAdaptor::template base<F, G>
    {
        typedef detail::compressed_pair<F, G> base;
        typedef apply fit_rewritable_tag;

        FIT_RETURNS_CLASS(apply);

        template<class... Ts>
        constexpr FIT_SFINAE_MANUAL_RESULT(typename BinaryAdaptor::apply, id_<const F&>, id_<const G&>, id_<Ts>...) 
        operator()(Ts&&... xs) const FIT_SFINAE_MANUAL_RETURNS
        (
            FIT_RETURNS_CONSTRUCT(typename BinaryAdaptor::apply)()(
                FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->first(xs...)),
                FIT_MANGLE_CAST(const G&)(FIT_CONST_THIS->second(xs...)),
                FIT_FORWARD(Ts)(xs)...
            )
        );

        FIT_INHERIT_DEFAULT(apply, F, G)

        FIT_INHERIT_CONSTRUCTOR(apply, base)
    };

    template<class F, class G, class... Fs>
    struct apply<F, G, Fs...>
    : apply<F, apply<G, Fs...>>
    {
        typedef apply<F, apply<G, Fs...>> base;
        typedef apply<G, Fs...> tail_base;

        FIT_INHERIT_DEFAULT(apply, base)
        
        template<class X, class... Xs, 
            FIT_ENABLE_IF_CONSTRUCTIBLE(base, X, tail_base), 
            FIT_ENABLE_IF_CONSTRUCTIBLE(tail_base, Xs...)>
        constexpr apply(X&& f1, Xs&& ... fs) 
        noexcept(FIT_IS_NOTHROW_CONSTRUCTIBLE(base, X&&, tail_base) && FIT_IS_NOTHROW_CONSTRUCTIBLE(tail_base, Xs&&...))
        : base(FIT_FORWARD(X)(f1), tail_base(FIT_FORWARD(Xs)(fs)...))
        {}

        template<class X, 
            FIT_ENABLE_IF_CONSTRUCTIBLE(base, X)>
        constexpr apply(X&& f1) 
        FIT_NOEXCEPT_CONSTRUCTIBLE(base, X&&)
        : base(FIT_FORWARD(X)(f1))
        {}
    };
#endif
};

}} // namespace fit

#endif
