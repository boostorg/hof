/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    conditional.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_HOF_GUARD_FUNCTION_CONDITIONAL_H
#define BOOST_HOF_GUARD_FUNCTION_CONDITIONAL_H

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
///     using namespace boost::hof;
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
/// 
/// References
/// ----------
/// 
/// * [POO51](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0051r2.pdf) - Proposal for C++
///   Proposal for C++ generic overload function
/// * [Conditional overloading](<Conditional overloading>)
/// 

#include <boost/hof/reveal.hpp>
#include <boost/hof/detail/compressed_pair.hpp>
#include <boost/hof/detail/callable_base.hpp>
#include <boost/hof/detail/delegate.hpp>
#include <boost/hof/detail/join.hpp>
#include <boost/hof/detail/seq.hpp>
#include <boost/hof/detail/make.hpp>
#include <boost/hof/detail/static_const_var.hpp>

namespace boost { namespace hof {

namespace detail {

template<class F1, class F2>
struct basic_conditional_adaptor : F1, F2
{
    BOOST_HOF_INHERIT_DEFAULT(basic_conditional_adaptor, F1, F2)

    template<class A, class B,
        BOOST_HOF_ENABLE_IF_CONVERTIBLE(A, F1),
        BOOST_HOF_ENABLE_IF_CONVERTIBLE(B, F2)>
    constexpr basic_conditional_adaptor(A&& f1, B&& f2)
    noexcept(BOOST_HOF_IS_NOTHROW_CONSTRUCTIBLE(F1, A&&) && BOOST_HOF_IS_NOTHROW_CONSTRUCTIBLE(F2, B&&))
    : F1(BOOST_HOF_FORWARD(A)(f1)), F2(BOOST_HOF_FORWARD(B)(f2))
    {}

    template<class X,
        class=typename std::enable_if<
        BOOST_HOF_IS_CONVERTIBLE(X, F1) && 
        BOOST_HOF_IS_DEFAULT_CONSTRUCTIBLE(F2)
    >::type>
    constexpr basic_conditional_adaptor(X&& x) 
    BOOST_HOF_NOEXCEPT_CONSTRUCTIBLE(F1, X&&)
    : F1(BOOST_HOF_FORWARD(X)(x))
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

    BOOST_HOF_RETURNS_CLASS(basic_conditional_adaptor);

    template<class... Ts, class F=typename select<Ts...>::type>
    constexpr BOOST_HOF_SFINAE_RESULT(typename select<Ts...>::type, id_<Ts>...) 
    operator()(Ts && ... xs) const
    BOOST_HOF_SFINAE_RETURNS
    (
        BOOST_HOF_RETURNS_STATIC_CAST(const F&)(*BOOST_HOF_CONST_THIS)(BOOST_HOF_FORWARD(Ts)(xs)...)
    );
};

template <class F1, class F2>
constexpr const F1& which(std::true_type, const F1& f1, const F2&) noexcept
{ 
    return f1; 
}

template <class F1, class F2>
constexpr const F2& which(std::false_type, const F1&, const F2& f2) noexcept
{ 
    return f2; 
}

template<class F1, class F2>
struct conditional_kernel : compressed_pair<F1, F2>
{
    typedef compressed_pair<F1, F2> base;
    BOOST_HOF_INHERIT_CONSTRUCTOR(conditional_kernel, base)

    template<class... Ts>
    struct select
    : std::conditional
    <
        is_callable<F1, Ts...>::value, 
        F1,
        F2
    >
    {};

    BOOST_HOF_RETURNS_CLASS(conditional_kernel);

    template<class... Ts, class PickFirst=is_callable<F1, Ts...>>
    constexpr BOOST_HOF_SFINAE_RESULT(typename select<Ts...>::type, id_<Ts>...) 
    operator()(Ts && ... xs) const
    BOOST_HOF_SFINAE_RETURNS
    (
        boost::hof::detail::which(
            BOOST_HOF_RETURNS_CONSTRUCT(PickFirst)(),
            BOOST_HOF_MANGLE_CAST(const F1&)(BOOST_HOF_CONST_THIS->first(xs...)),
            BOOST_HOF_MANGLE_CAST(const F2&)(BOOST_HOF_CONST_THIS->second(xs...))
        )
        (BOOST_HOF_FORWARD(Ts)(xs)...)
    );
};
}

template<class F, class... Fs>
struct conditional_adaptor 
: detail::conditional_kernel<F, BOOST_HOF_JOIN(conditional_adaptor, Fs...) >
{
    typedef conditional_adaptor fit_rewritable_tag;
    typedef BOOST_HOF_JOIN(conditional_adaptor, Fs...) kernel_base;
    typedef detail::conditional_kernel<F, kernel_base > base;

    BOOST_HOF_INHERIT_DEFAULT(conditional_adaptor, base)

    template<class X, class... Xs, 
        BOOST_HOF_ENABLE_IF_CONSTRUCTIBLE(base, X, kernel_base), 
        BOOST_HOF_ENABLE_IF_CONSTRUCTIBLE(kernel_base, Xs...)>
    constexpr conditional_adaptor(X&& f1, Xs&& ... fs) 
    noexcept(BOOST_HOF_IS_NOTHROW_CONSTRUCTIBLE(base, X&&, kernel_base) && BOOST_HOF_IS_NOTHROW_CONSTRUCTIBLE(kernel_base, Xs&&...))
    : base(BOOST_HOF_FORWARD(X)(f1), kernel_base(BOOST_HOF_FORWARD(Xs)(fs)...))
    {}

    template<class X, class... Xs, 
        BOOST_HOF_ENABLE_IF_CONSTRUCTIBLE(base, X)>
    constexpr conditional_adaptor(X&& f1) 
    BOOST_HOF_NOEXCEPT_CONSTRUCTIBLE(base, X&&)
    : base(BOOST_HOF_FORWARD(X)(f1))
    {}

    struct failure
    : failure_for<F, Fs...>
    {};
};

template<class F>
struct conditional_adaptor<F> : F
{
    typedef conditional_adaptor fit_rewritable_tag;
    BOOST_HOF_INHERIT_CONSTRUCTOR(conditional_adaptor, F);

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
    BOOST_HOF_INHERIT_CONSTRUCTOR(conditional_adaptor, base);

    struct failure
    : failure_for<F1, F2>
    {};
};

BOOST_HOF_DECLARE_STATIC_VAR(conditional, detail::make<conditional_adaptor>);

}} // namespace boost::hof

#endif
