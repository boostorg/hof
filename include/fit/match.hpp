/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    match.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_OVERLOAD_H
#define FIT_GUARD_FUNCTION_OVERLOAD_H

/// match
/// =====
/// 
/// Description
/// -----------
/// 
/// The `match` function adaptor combines several functions together and
/// resolves which one should be called by using C++ overload resolution. This
/// is different than the [`conditional`](conditional.md) adaptor which resolves
/// them based on order.
/// 
/// Synopsis
/// --------
/// 
///     template<class... Fs>
///     constexpr match_adaptor<Fs...> match(Fs...fs);
/// 
/// Requirements
/// ------------
/// 
/// Fs must be:
/// 
/// * [Callable](concepts.md#callable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     struct int_class
///     {
///         int operator()(int) const
///         {
///             return 1;
///         }
///     };
/// 
///     struct foo
///     {};
/// 
///     struct foo_class
///     {
///         foo operator()(foo) const
///         {
///             return foo();
///         }
///     };
/// 
///     typedef match_adaptor<int_class, foo_class> fun;
/// 
///     static_assert(std::is_same<int, decltype(fun()(1))>::value, "Failed match");
///     static_assert(std::is_same<foo, decltype(fun()(foo()))>::value, "Failed match");
/// 

#include <fit/reveal.hpp>
#include <fit/detail/callable_base.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/static_const_var.hpp>

namespace fit {

template<class...Fs> struct match_adaptor;
 
template<class F, class...Fs>
struct match_adaptor<F, Fs...> : detail::callable_base<F>, match_adaptor<Fs...>
{
    typedef match_adaptor<Fs...> base;
    typedef match_adaptor fit_rewritable_tag;

    struct failure
    : failure_for<detail::callable_base<F>, Fs...>
    {};

    FIT_INHERIT_DEFAULT(match_adaptor, detail::callable_base<F>, base);

    template<class X, class... Xs, FIT_ENABLE_IF_CONVERTIBLE(X, detail::callable_base<F>), FIT_ENABLE_IF_CONSTRUCTIBLE(base, Xs...)>
    constexpr match_adaptor(X&& f1, Xs&& ... fs) 
    : detail::callable_base<F>(fit::forward<X>(f1)), base(fit::forward<Xs>(fs)...)
    {}

    using F::operator();
    using base::operator();
};

template<class F>
struct match_adaptor<F> : detail::callable_base<F>
{
    typedef detail::callable_base<F> base;
    typedef match_adaptor fit_rewritable_tag;
    using F::operator();

    FIT_INHERIT_CONSTRUCTOR(match_adaptor, detail::callable_base<F>);
};

FIT_DECLARE_STATIC_VAR(match, detail::make<match_adaptor>);

} // namespace fit

#endif
