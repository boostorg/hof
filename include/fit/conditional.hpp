/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    conditional.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_CONDITIONAL_H
#define FIT_GUARD_FUNCTION_CONDITIONAL_H

/// conditional
/// ===========
/// 
/// Description
/// -----------
/// 
/// The `conditional` function adaptor combines several functions together. If
/// the first function can not be called, then it will try to call the next
/// function. This can be very useful when overloading functions using
/// template constraints(such as with `enable_if`).
/// 
/// Note: This is different than the [`match`](match.md) function adaptor, which
/// can lead to ambiguities. Instead, `conditional` will call the first function
/// that is callable, regardless if there is another function that could be
/// called as well.
/// 
/// Synopsis
/// --------
/// 
///     template<class... Fs>
///     constexpr conditional_adaptor<Fs...> conditional(Fs... fs);
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
///     struct for_ints
///     {
///         void operator()(int) const
///         {
///             printf("Int\n");
///         }
///     };
/// 
///     struct for_floats
///     {
///         void operator()(int) const
///         {
///             printf("Float\n");
///         }
///     };
/// 
///     conditional(for_ints(), for_floats())(3.0);
/// 
/// This will print `Int` because the `for_floats` function object won't ever be
/// called. Due to the conversion rules in C++, the `for_ints` function can be
/// called on floats, so it is chosen by `conditional` first, even though
/// `for_floats` is a better match.
/// 
/// So, the order of the functions in the `conditional_adaptor` are very important
/// to how the function is chosen.

#include <fit/reveal.hpp>
#include <fit/detail/callable_base.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/join.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/static_const_var.hpp>

namespace fit {

namespace detail {

template<int N>
struct rank : rank<N-1>
{};

template<>
struct rank<0>
{};

template<int N, class...Fs> struct conditional_adaptor_base;
 
template<int N, class F, class...Fs>
struct conditional_adaptor_base<N, F, Fs...> : conditional_adaptor_base<N, F>, conditional_adaptor_base<N-1, Fs...>
{
    typedef conditional_adaptor_base<N-1, Fs...> base;
    typedef conditional_adaptor_base<N, F> single_base;

    FIT_INHERIT_DEFAULT(conditional_adaptor_base, single_base, base);

    template<class X, class... Xs, FIT_ENABLE_IF_CONVERTIBLE(X, single_base), FIT_ENABLE_IF_CONSTRUCTIBLE(base, Xs...)>
    constexpr conditional_adaptor_base(X&& f1, Xs&& ... fs) 
    : single_base(fit::forward<X>(f1)), base(fit::forward<Xs>(fs)...)
    {}

    using base::operator();
    using single_base::operator();
};

template<int N, class F>
struct conditional_adaptor_base<N, F> : detail::callable_base<F>
{
    typedef detail::callable_base<F> base;

    FIT_INHERIT_CONSTRUCTOR(conditional_adaptor_base, detail::callable_base<F>);

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(conditional_adaptor_base);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(const detail::callable_base<F>&, id_<Ts>...) 
    operator()(rank<N>, Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        (FIT_MANGLE_CAST(const detail::callable_base<F>&)(FIT_CONST_THIS->base_function(xs...)))(fit::forward<Ts>(xs)...)
    );
};

}

template<class... Fs>
struct conditional_adaptor 
: detail::conditional_adaptor_base<sizeof...(Fs), Fs...>
{
    typedef conditional_adaptor fit_rewritable_tag;
    typedef detail::conditional_adaptor_base<sizeof...(Fs), Fs...> base;
    typedef detail::rank<sizeof...(Fs)> rank_type;

    FIT_INHERIT_CONSTRUCTOR(conditional_adaptor, base);

    struct failure
    : failure_for<Fs...>
    {};

    template<class... Ts>
    constexpr const base& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(conditional_adaptor);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(const base&, id_<rank_type>, id_<Ts>...) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        (FIT_MANGLE_CAST(const base&)(FIT_CONST_THIS->base_function(xs...)))(rank_type(), fit::forward<Ts>(xs)...)
    );
};

FIT_DECLARE_STATIC_VAR(conditional, detail::make<conditional_adaptor>);

} // namespace fit

#endif
