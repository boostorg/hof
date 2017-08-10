/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    binary.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_DETAIL_BUILDER_BINARY_HPP
#define FIT_GUARD_DETAIL_BUILDER_BINARY_HPP

#include <fit/detail/builder.hpp>
#include <fit/detail/builder/unary.hpp>
#include <fit/detail/compressed_pair.hpp>

namespace fit { namespace detail {


template<class BinaryAdaptor, class UnaryAdaptor=void>
struct binary_adaptor_builder
{
    template<class... Fs>
    struct apply;

    template<class F>
    struct apply<F>
    : unary_adaptor_builder<UnaryAdaptor>::template apply<F>
    {
        typedef typename unary_adaptor_builder<UnaryAdaptor>::template apply<F> base;

        FIT_INHERIT_CONSTRUCTOR(apply, base)
    };

    template<class F, class G>
    struct apply<F, G>
    : detail::compressed_pair<detail::callable_base<F>, detail::callable_base<G>>, BinaryAdaptor::template base<detail::callable_base<F>, detail::callable_base<G>>
    {
        typedef detail::compressed_pair<detail::callable_base<F>, detail::callable_base<G>> base;
        typedef apply fit_rewritable_tag;

        FIT_RETURNS_CLASS(apply);

        template<class... Ts>
        constexpr FIT_SFINAE_RESULT(typename BinaryAdaptor::apply, id_<const callable_base<F>&>, id_<const callable_base<G>&>, id_<Ts>...) 
        operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
        (
            FIT_RETURNS_CONSTRUCT(typename BinaryAdaptor::apply)()(
                FIT_MANGLE_CAST(const callable_base<F>&)(FIT_CONST_THIS->first(xs...)),
                FIT_MANGLE_CAST(const callable_base<G>&)(FIT_CONST_THIS->second(xs...)),
                FIT_FORWARD(Ts)(xs)...
            )
        );

        FIT_INHERIT_CONSTRUCTOR(apply, base)
    };
};

}} // namespace fit

#endif
