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
/// function. This can very useful when overloading functions using template
/// constraints(such as with `enable_if`).
/// 
/// Note: This is different than the `match` function adaptor, which can lead
/// to ambiguities. Instead, `conditional` will call the first function that
/// is callable, regardless if there is another function that could be called
/// as well. 
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
///     FunctionObject
///     MoveConstructible
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

#include <fit/is_callable.h>
#include <fit/reveal.h>
#include <fit/detail/result_of.h>
#include <fit/detail/delegate.h>
#include <fit/detail/join.h>
#include <fit/detail/make.h>
#include <fit/detail/static_const_var.h>
#include <type_traits>

namespace fit {

namespace detail {

template<class F1, class F2>
struct conditional_kernel : F1, F2
{
    FIT_INHERIT_DEFAULT(conditional_kernel, F1, F2)

    template<class A, class B,
        FIT_ENABLE_IF_CONVERTIBLE(A, F1),
        FIT_ENABLE_IF_CONVERTIBLE(B, F2)>
    constexpr conditional_kernel(A&& f1, B&& f2) : F1(fit::forward<A>(f1)), F2(fit::forward<B>(f2))
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

    template<class... Ts>
    constexpr const typename select<Ts...>::type& select_function() const
    {
        return *this;
    }

    FIT_RETURNS_CLASS(conditional_kernel);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(typename select<Ts...>::type, id_<Ts>...) 
    operator()(Ts && ... x) const
    FIT_SFINAE_RETURNS(FIT_CONST_THIS->select_function<Ts&&...>()(fit::forward<Ts>(x)...));
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
    : base(fit::forward<X>(f1), kernel_base(fit::forward<Xs>(fs)...))
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

FIT_DECLARE_STATIC_VAR(conditional, detail::make<conditional_adaptor>);

}

#endif
