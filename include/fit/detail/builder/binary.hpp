/*=============================================================================
    Copyright (c) 2017 Paul Fultz II
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
    struct adaptor;

    template<class F>
    struct adaptor<F>
    : unary_adaptor_builder<UnaryAdaptor>::template adaptor<F>
    {
        typedef typename unary_adaptor_builder<UnaryAdaptor>::template adaptor<F> base;

        FIT_INHERIT_CONSTRUCTOR(adaptor, base)
    };

    template<class F, class G>
    struct adaptor<F, G>
    : 
        detail::compressed_pair<F, G>, 
        BinaryAdaptor::template base<F, G>
    {
        typedef detail::compressed_pair<F, G> base;
        typedef adaptor fit_rewritable_tag;

        FIT_RETURNS_CLASS(adaptor);

        template<class... Ts>
        constexpr FIT_SFINAE_RESULT(typename BinaryAdaptor::apply, id_<const F&>, id_<const G&>, id_<Ts>...) 
        operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
        (
            FIT_RETURNS_CONSTRUCT(typename BinaryAdaptor::apply)()(
                FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->first(xs...)),
                FIT_MANGLE_CAST(const G&)(FIT_CONST_THIS->second(xs...)),
                FIT_FORWARD(Ts)(xs)...
            )
        );

        FIT_INHERIT_CONSTRUCTOR(adaptor, base)
    };
};

}} // namespace fit

#endif
