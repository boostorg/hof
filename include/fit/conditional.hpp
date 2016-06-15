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
/// * [ConstCallable](ConstCallable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <iostream>
///     using namespace fit;
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
///         void operator()(float) const
///         {
///             printf("Float\n");
///         }
///     };
/// 
///     int main() {
///         conditional(for_ints(), for_floats())(3.0);
///     }
/// 
/// This will print `Int` because the `for_floats` function object won't ever be
/// called. Due to the conversion rules in C++, the `for_ints` function can be
/// called on floats, so it is chosen by `conditional` first, even though
/// `for_floats` is a better match.
/// 
/// So, the order of the functions in the `conditional_adaptor` are very important
/// to how the function is chosen.

#include <fit/reveal.hpp>
#include <fit/detail/compressed_pair.hpp>
#include <fit/detail/callable_base.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/join.hpp>
#include <fit/detail/seq.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/static_const_var.hpp>

namespace fit {

namespace detail {

template<class F1, class F2>
struct basic_conditional_adaptor : F1, F2
{
    FIT_INHERIT_DEFAULT(basic_conditional_adaptor, F1, F2)

    template<class A, class B,
        FIT_ENABLE_IF_CONVERTIBLE(A, F1),
        FIT_ENABLE_IF_CONVERTIBLE(B, F2)>
    constexpr basic_conditional_adaptor(A&& f1, B&& f2) : F1(FIT_FORWARD(A)(f1)), F2(FIT_FORWARD(B)(f2))
    {}

    template<class X,
        class=typename std::enable_if<
        FIT_IS_CONVERTIBLE(X, F1) && 
        FIT_IS_DEFAULT_CONSTRUCTIBLE(F2)
    >::type>
    constexpr basic_conditional_adaptor(X&& x) : F1(FIT_FORWARD(X)(x))
    {} 

    template<class... Ts>
    struct select
    : std::conditional
    <
        is_callable<F1, Ts...>::value, 
        F1,
        F2
    >
    {};

    FIT_RETURNS_CLASS(basic_conditional_adaptor);

    template<class... Ts, class F=typename select<Ts...>::type>
    constexpr FIT_SFINAE_RESULT(typename select<Ts...>::type, id_<Ts>...) 
    operator()(Ts && ... xs) const
    FIT_SFINAE_RETURNS
    (
        FIT_RETURNS_STATIC_CAST(const F&)(*FIT_CONST_THIS)(FIT_FORWARD(Ts)(xs)...)
    );
};

template <class F1, class F2>
constexpr const F1& which(holder<F1>, const F1& f1, const F2&) 
{ 
    return f1; 
}

template <class F1, class F2>
constexpr const F2& which(holder<F2>, const F1&, const F2& f2) 
{ 
    return f2; 
}

template<class F1, class F2>
struct conditional_kernel : compressed_pair<F1, F2>
{
    typedef compressed_pair<F1, F2> base;
    FIT_INHERIT_CONSTRUCTOR(conditional_kernel, base)

    template<class... Ts>
    struct select
    : std::conditional
    <
        is_callable<F1, Ts...>::value, 
        F1,
        F2
    >
    {};

    FIT_RETURNS_CLASS(conditional_kernel);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(typename select<Ts...>::type, id_<Ts>...) 
    operator()(Ts && ... xs) const
    FIT_SFINAE_RETURNS
    (
        detail::which(
            FIT_RETURNS_CONSTRUCT(holder<typename select<Ts...>::type>)(),
            FIT_MANGLE_CAST(const F1&)(FIT_CONST_THIS->first(xs...)),
            FIT_MANGLE_CAST(const F2&)(FIT_CONST_THIS->second(xs...))
        )
        (FIT_FORWARD(Ts)(xs)...)
    );
};
}

template<class F, class... Fs>
struct conditional_adaptor 
: detail::conditional_kernel<F, FIT_JOIN(conditional_adaptor, Fs...) >
{
    typedef conditional_adaptor fit_rewritable_tag;
    typedef FIT_JOIN(conditional_adaptor, Fs...) kernel_base;
    typedef detail::conditional_kernel<F, kernel_base > base;

    FIT_INHERIT_DEFAULT(conditional_adaptor, base)

    template<class X, class... Xs, 
        FIT_ENABLE_IF_CONSTRUCTIBLE(base, X, kernel_base), 
        FIT_ENABLE_IF_CONSTRUCTIBLE(kernel_base, Xs...)>
    constexpr conditional_adaptor(X&& f1, Xs&& ... fs) 
    : base(FIT_FORWARD(X)(f1), kernel_base(FIT_FORWARD(Xs)(fs)...))
    {}

    template<class X, class... Xs, 
        FIT_ENABLE_IF_CONSTRUCTIBLE(base, X)>
    constexpr conditional_adaptor(X&& f1) 
    : base(FIT_FORWARD(X)(f1))
    {}

    struct failure
    : failure_for<F, Fs...>
    {};
};

template<class F>
struct conditional_adaptor<F> : F
{
    typedef conditional_adaptor fit_rewritable_tag;
    FIT_INHERIT_CONSTRUCTOR(conditional_adaptor, F);

    struct failure
    : failure_for<F>
    {};
};

template<class F1, class F2>
struct conditional_adaptor<F1, F2> 
: detail::conditional_kernel<F1, F2>
{
    typedef detail::conditional_kernel<F1, F2> base;
    typedef conditional_adaptor fit_rewritable_tag;
    FIT_INHERIT_CONSTRUCTOR(conditional_adaptor, base);

    struct failure
    : failure_for<F1, F2>
    {};
};

FIT_DECLARE_STATIC_VAR(conditional, detail::make<conditional_adaptor>);

} // namespace fit

#endif
