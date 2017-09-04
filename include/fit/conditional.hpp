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
/// 
/// References
/// ----------
/// 
/// * [POO51](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0051r2.pdf) - Proposal for C++
///   Proposal for C++ generic overload function
/// * [Conditional overloading](<Conditional overloading>)
/// 

#include <fit/detail/builder.hpp>
#include <fit/detail/builder/fold.hpp>
#include <fit/reveal.hpp>

namespace fit {

namespace detail {

template<class F1, class F2>
struct basic_conditional_adaptor : F1, F2
{
    FIT_INHERIT_DEFAULT(basic_conditional_adaptor, F1, F2)

    template<class A, class B,
        FIT_ENABLE_IF_CONVERTIBLE(A, F1),
        FIT_ENABLE_IF_CONVERTIBLE(B, F2)>
    constexpr basic_conditional_adaptor(A&& f1, B&& f2)
    noexcept(FIT_IS_NOTHROW_CONSTRUCTIBLE(F1, A&&) && FIT_IS_NOTHROW_CONSTRUCTIBLE(F2, B&&))
    : F1(FIT_FORWARD(A)(f1)), F2(FIT_FORWARD(B)(f2))
    {}

    template<class X,
        class=typename std::enable_if<
        FIT_IS_CONVERTIBLE(X, F1) && 
        FIT_IS_DEFAULT_CONSTRUCTIBLE(F2)
    >::type>
    constexpr basic_conditional_adaptor(X&& x) 
    FIT_NOEXCEPT_CONSTRUCTIBLE(F1, X&&)
    : F1(FIT_FORWARD(X)(x))
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
constexpr const F1& which(std::true_type, const F1& f1, const F2&) noexcept
{ 
    return f1; 
}

template <class F1, class F2>
constexpr const F2& which(std::false_type, const F1&, const F2& f2) noexcept
{ 
    return f2; 
}

struct conditional_adaptor_base
{
    template<class F, class G>
    struct base
    {
        struct failure
        : failure_for<F, G>
        {};
    };

    struct apply
    {
        
        template<class F1, class F2, class... Ts>
        struct select
        : std::conditional
        <
            is_callable<F1, Ts...>::value, 
            F1,
            F2
        >
        {};

        template<class F1, class F2, class... Ts, class PickFirst=is_callable<F1, Ts...>>
        constexpr FIT_SFINAE_RESULT(typename select<F1, F2, Ts...>::type, id_<Ts>...) 
        operator()(F1&& f1, F2&& f2, Ts && ... xs) const
        FIT_SFINAE_RETURNS
        (
            detail::which(
                FIT_RETURNS_CONSTRUCT(PickFirst)(),
                FIT_FORWARD(F1)(f1),
                FIT_FORWARD(F2)(f2)
            )
            (FIT_FORWARD(Ts)(xs)...)
        );
    };
};
}

FIT_DECLARE_ADAPTOR(conditional, detail::fold_adaptor_builder<detail::conditional_adaptor_base>)

} // namespace fit

#endif
