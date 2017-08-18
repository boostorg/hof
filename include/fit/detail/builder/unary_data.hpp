/*=============================================================================
    Copyright (c) 2017 Paul Fultz II
    unary_data.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_DETAIL_BUILDER_unary_data_HPP
#define FIT_GUARD_DETAIL_BUILDER_unary_data_HPP

#include <fit/detail/builder.hpp>
#include <fit/detail/builder/unary.hpp>
#include <fit/detail/compressed_pair.hpp>

namespace fit { namespace detail {

template<class UnaryDataAdaptor, class UnaryAdaptor=void>
struct unary_data_adaptor_builder
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
    : 
        detail::compressed_pair<detail::callable_base<F>, G>, 
        UnaryDataAdaptor::template base<detail::callable_base<F>, G>
    {
        typedef detail::compressed_pair<detail::callable_base<F>, G> base;
        typedef apply fit_rewritable1_tag;

        FIT_RETURNS_CLASS(apply);

        template<class... Ts>
        constexpr FIT_SFINAE_RESULT(typename UnaryDataAdaptor::apply, id_<const callable_base<F>&>, id_<const G&>, id_<Ts>...) 
        operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
        (
            FIT_RETURNS_CONSTRUCT(typename UnaryDataAdaptor::apply)()(
                FIT_MANGLE_CAST(const callable_base<F>&)(FIT_CONST_THIS->first(xs...)),
                FIT_MANGLE_CAST(const G&)(FIT_CONST_THIS->second(xs...)),
                FIT_FORWARD(Ts)(xs)...
            )
        );

        FIT_INHERIT_CONSTRUCTOR(apply, base)
    };
};

}} // namespace fit

#endif
