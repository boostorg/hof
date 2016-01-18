/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    decorate.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_DECORATE_H
#define FIT_GUARD_DECORATE_H

/// decorate
/// ========
/// 
/// Description
/// -----------
/// 
/// The `decorate` function adaptor helps create simple function decorators. 
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr decorate_adaptor<F> decorate(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(decorate(f)(x)(g)(xs...) == f(x, g, xs...));
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [Callable](concepts.md#callable)
/// * MoveConstructible
/// 

#include <fit/reveal.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/callable_base.hpp>
#include <fit/detail/static_const_var.hpp>
#include <fit/detail/compressed_pair.hpp>

namespace fit { namespace detail {

template<class F, class T, class D>
struct decorator_invoke
: compressed_pair<F, T>, D
{
    typedef compressed_pair<F, T> base;

    template<class X1, class X2, class X3>
    constexpr decorator_invoke(X1&& x1, X2&& x2, X3&& x3)
    : base(fit::forward<X1>(x1), fit::forward<X2>(x2)), D(fit::forward<X3>(x3))
    {}

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return this->first(xs...);
    }

    template<class... Ts>
    constexpr const T& get_data(Ts&&... xs) const
    {
        return this->second(xs...);
    }

    template<class... Ts>
    constexpr const D& get_decorator(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(decorator_invoke);

    struct decorator_invoke_failure
    {
        template<class Failure>
        struct apply
        {
            template<class... Ts>
            struct of
            : Failure::template of<const T&, const F&, Ts...>
            {};
        };
    };

    struct failure
    : failure_map<decorator_invoke_failure, D>
    {};

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(const D&, id_<const T&>, id_<const F&>, id_<Ts>...) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        FIT_MANGLE_CAST(const D&)(FIT_CONST_THIS->get_decorator(xs...))(
            FIT_MANGLE_CAST(const T&)(FIT_CONST_THIS->get_data(xs...)),
            FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...)),
            fit::forward<Ts>(xs)...
        )
    );
};

template<class T, class D>
struct decoration
: compressed_pair<T, D>
{
    typedef compressed_pair<T, D> base;
    FIT_INHERIT_CONSTRUCTOR(decoration, base)

    template<class... Ts>
    constexpr const D& get_decorator(Ts&&... xs) const
    {
        return this->second(xs...);
    }

    template<class... Ts>
    constexpr const T& get_data(Ts&&... xs) const
    {
        return this->first(xs...);
    }

    template<class F>
    constexpr decorator_invoke<detail::callable_base<F>, T, D> operator()(F f) const
    {
        return decorator_invoke<detail::callable_base<F>, T, D>(fit::move(f), this->get_data(f), this->get_decorator(f));
    }
};

}

template<class F>
struct decorate_adaptor : detail::callable_base<F>
{
    typedef decorate_adaptor fit_rewritable1_tag;
    FIT_INHERIT_CONSTRUCTOR(decorate_adaptor, detail::callable_base<F>)

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    // TODO: Add predicate for constraints

    template<class T>
    constexpr detail::decoration<T, detail::callable_base<F>> operator()(T x) const
    {
        return detail::decoration<T, detail::callable_base<F>>(fit::move(x), this->base_function(x));
    }

};

FIT_DECLARE_STATIC_VAR(decorate, detail::make<decorate_adaptor>);

} // namespace fit

#endif
