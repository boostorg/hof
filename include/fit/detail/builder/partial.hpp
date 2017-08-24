/*=============================================================================
    Copyright (c) 2017 Paul Fultz II
    partial.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_DETAIL_PARTIAL_HPP
#define FIT_GUARD_DETAIL_PARTIAL_HPP

#include <fit/detail/builder.hpp>
#include <fit/detail/builder/unary.hpp>
#include <fit/detail/compressed_pair.hpp>
#include <fit/reveal.hpp>
#include <fit/function_param_limit.hpp>

namespace fit { namespace detail {

template<class Adaptor>
struct partial_adaptor_builder_base
{
    template<class F>
    struct base
    {
        struct failure
        : failure_for<F>
        {};
    };

    struct apply
    {
        template<class F, class... Ts>
        constexpr FIT_SFINAE_RESULT(F&&, id_<Ts>...) 
        operator()(F&& f, Ts&&... xs) const FIT_SFINAE_RETURNS
        (
            FIT_FORWARD(F)(f)(FIT_FORWARD(Ts)(xs)...)
        );

        template<class F, class... Ts, class=typename std::enable_if<
            ((sizeof...(Ts) < fit::function_param_limit<F>::value) && !fit::detail::can_be_called<F, Ts...>::value)
        >::type>
        constexpr FIT_SFINAE_RESULT(typename Adaptor::apply, id_<F&&>, id_<Ts>...) 
        operator()(F&& f, Ts&&... xs) const FIT_SFINAE_RETURNS
        (
            FIT_RETURNS_CONSTRUCT(typename Adaptor::apply)()(
                FIT_FORWARD(F)(f), 
                FIT_FORWARD(Ts)(xs)...
            )
        );
    };
};


template<class UnaryAdaptor, class BinaryAdaptor=void>
struct partial_adaptor_builder
{
    template<class... Fs>
    struct apply;

    template<class F>
    struct apply<F>
    : unary_adaptor_builder<partial_adaptor_builder_base<UnaryAdaptor>>::template apply<F>
    {
        typedef typename unary_adaptor_builder<partial_adaptor_builder_base<UnaryAdaptor>>::template apply<F> base;

        FIT_INHERIT_CONSTRUCTOR(apply, base)
    };

    // template<class F, class G>
    // struct apply<F, G>
    // : 
    //     detail::compressed_pair<F, G>, 
    //     BinaryAdaptor::template base<F, G>
    // {
    //     typedef detail::compressed_pair<F, G> base;
    //     typedef apply fit_rewritable_tag;

    //     FIT_RETURNS_CLASS(apply);

    //     template<class... Ts>
    //     constexpr FIT_SFINAE_RESULT(typename BinaryAdaptor::apply, id_<const F&>, id_<const G&>, id_<Ts>...) 
    //     operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    //     (
    //         FIT_RETURNS_CONSTRUCT(typename BinaryAdaptor::apply)()(
    //             FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->first(xs...)),
    //             FIT_MANGLE_CAST(const G&)(FIT_CONST_THIS->second(xs...)),
    //             FIT_FORWARD(Ts)(xs)...
    //         )
    //     );

    //     FIT_INHERIT_CONSTRUCTOR(apply, base)
    // };
};

}} // namespace fit

#endif
