/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    unary.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_DETAIL_BUILDER_UNARY_HPP
#define FIT_GUARD_DETAIL_BUILDER_UNARY_HPP

#include <fit/detail/builder.hpp>

namespace fit { namespace detail {

template<class Adaptor>
struct unary_adaptor_builder
{
    template<class F>
    struct apply
    : F, Adaptor::template base<F>
    {
        typedef F base;
        typedef apply fit_rewritable1_tag;

        template<class... Ts>
        constexpr const F& base_function(Ts&&... xs) const noexcept
        {
            return always_ref(*this)(xs...);
        }

        FIT_RETURNS_CLASS(apply);

        template<class... Ts>
        constexpr FIT_SFINAE_RESULT(typename Adaptor::apply, id_<const F&>, id_<Ts>...) 
        operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
        (
            FIT_RETURNS_CONSTRUCT(typename Adaptor::apply)()(
                FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...)),
                FIT_FORWARD(Ts)(xs)...
            )
        );

        FIT_INHERIT_CONSTRUCTOR(apply, base)
    };
};

}} // namespace fit

#endif
