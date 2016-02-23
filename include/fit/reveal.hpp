/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    reveal.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_REVEAL_H
#define FIT_GUARD_FUNCTION_REVEAL_H

/// reveal
/// ======
/// 
/// Description
/// -----------
/// 
/// The `reveal` function adaptor helps shows the error messages that get
/// masked on some compilers. Sometimes an error in a function that causes a
/// substitution failure, will remove the function from valid overloads. On
/// compilers without a backtrace for substitution failure, this will mask the
/// error inside the function. The `reveal` adaptor will expose these error
/// messages while still keeping the function SFINAE-friendly.
/// 
/// Sample
/// ------
/// 
/// If we take the `print` example from the quick start guide like this:
/// 
///     namespace adl {
/// 
///     using std::begin;
/// 
///     template<class R>
///     auto adl_begin(R&& r) FIT_RETURNS(begin(r));
///     }
/// 
///     FIT_STATIC_LAMBDA_FUNCTION(for_each_tuple) = [](const auto& sequence, auto f) FIT_RETURNS
///     (
///         fit::unpack(fit::by(f))(sequence)
///     );
/// 
///     auto print = fit::fix(fit::conditional(
///         [](auto, const auto& x) -> decltype(std::cout << x, void())
///         {
///             std::cout << x << std::endl;
///         },
///         [](auto self, const auto& range) -> decltype(self(*adl::adl_begin(range)), void())
///         {
///             for(const auto& x:range) self(x);
///         },
///         [](auto self, const auto& tuple) -> decltype(for_each_tuple(tuple, self), void())
///         {
///             return for_each_tuple(tuple, self);
///         }
///     ));
/// 
/// Which prints numbers and vectors:
/// 
///     print(5);
/// 
///     std::vector<int> v = { 1, 2, 3, 4 };
///     print(v);
/// 
/// However, if we pass a type that can't be printed, we get an error like
/// this:
/// 
///     print.cpp:49:5: error: no matching function for call to object of type 'fit::fix_adaptor<fit::conditional_adaptor<(lambda at print.cpp:29:9), (lambda at print.cpp:33:9), (lambda at print.cpp:37:9)> >'
///         print(foo{});
///         ^~~~~
///     fix.hpp:158:5: note: candidate template ignored: substitution failure [with Ts = <foo>]: no matching function for call to object of type 'const fit::conditional_adaptor<(lambda at
///           print.cpp:29:9), (lambda at print.cpp:33:9), (lambda at print.cpp:37:9)>'
///         operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
/// 
/// Which is short and gives very little information why it can't be called.
/// It doesn't even show the overloads that were try. However, using the
/// `reveal` adaptor we can get more info about the error like this:
/// 
///     print.cpp:49:5: error: no matching function for call to object of type 'fit::reveal_adaptor<fit::fix_adaptor<fit::conditional_adaptor<(lambda at print.cpp:29:9), (lambda at print.cpp:33:9),
///           (lambda at print.cpp:37:9)> >, fit::fix_adaptor<fit::conditional_adaptor<(lambda at print.cpp:29:9), (lambda at print.cpp:33:9), (lambda at print.cpp:37:9)> > >'
///         fit::reveal(print)(foo{});
///         ^~~~~~~~~~~~~~~~~~
///     reveal.hpp:149:20: note: candidate template ignored: substitution failure [with Ts = <foo>, $1 = void]: no matching function for call to object of type '(lambda at print.cpp:29:9)'
///         constexpr auto operator()(Ts&&... xs) const
///                        ^
///     reveal.hpp:149:20: note: candidate template ignored: substitution failure [with Ts = <foo>, $1 = void]: no matching function for call to object of type '(lambda at print.cpp:33:9)'
///         constexpr auto operator()(Ts&&... xs) const
///                        ^
///     reveal.hpp:149:20: note: candidate template ignored: substitution failure [with Ts = <foo>, $1 = void]: no matching function for call to object of type '(lambda at print.cpp:37:9)'
///         constexpr auto operator()(Ts&&... xs) const
///                        ^
///     fix.hpp:158:5: note: candidate template ignored: substitution failure [with Ts = <foo>]: no matching function for call to object of type 'const fit::conditional_adaptor<(lambda at
///           print.cpp:29:9), (lambda at print.cpp:33:9), (lambda at print.cpp:37:9)>'
///         operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
/// 
/// So now the error has a note for each of the lambda overloads it tried. Of
/// course this can be improved even further by providing custom reporting of
/// failures.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     reveal_adaptor<F> reveal(F f);
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [Callable](concepts.md#callable)
/// * MoveConstructible
/// 
/// Reporting Failures
/// ==================
/// 
/// By default, `reveal` reports the substitution failure by trying to call
/// the function. However, more detail expressions can be be reported from a
/// template alias by using `as_failure`. This is done by defining a nested
/// `failure` struct in the function object and then inheriting from
/// `as_failure`. Also multiple failures can be reported by using
/// `with_failures`.
/// 
/// Synopsis
/// --------
/// 
///     // Report failure by instantiating the Template
///     template<template<class...> class Template>
///     struct as_failure;
/// 
///     // Report multiple falures
///     template<class... Failures>
///     struct with_failures;
/// 
///     // Report the failure for each function
///     template<class... Fs>
///     struct failure_for;
/// 
///     // Get the failure of a function
///     template<class F>
///     struct get_failure;
/// 
/// Example
/// -------
/// 
///     struct sum_f
///     {
///         template<class T, class U>
///         using sum_failure = decltype(std::declval<T>()+std::declval<U>());
/// 
///         struct failure
///         : as_failure<sum_failure>
///         {};
/// 
///         template<class T, class U>
///         auto operator()(T x, U y) const FIT_RETURNS(x+y);
///     };
/// 
/// 

#include <fit/always.hpp>
#include <fit/returns.hpp>
#include <fit/is_callable.hpp>
#include <fit/identity.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/callable_base.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/holder.hpp>
#include <fit/detail/join.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/static_const_var.hpp>
#include <fit/detail/using.hpp>

#ifndef FIT_REVEAL_USE_TEMPLATE_ALIAS
#ifdef __clang__
#define FIT_REVEAL_USE_TEMPLATE_ALIAS 1
#else
#define FIT_REVEAL_USE_TEMPLATE_ALIAS 0
#endif
#endif

namespace fit { 

namespace detail {


template<class T, class=void>
struct has_failure
: std::false_type
{};

template<class T>
struct has_failure<T, typename holder<
    typename T::failure
>::type>
: std::true_type
{};

struct identity_failure
{
    template<class T>
    T operator()(T&& x);

    template<class T>
    static T&& val();
#if FIT_REVEAL_USE_TEMPLATE_ALIAS
    template<template<class...> class Template, class... Ts>
    FIT_USING(defer, Template<Ts...>);
#else
    template<template<class...> class Template, class... Ts>
    static auto defer(Ts&&...) -> Template<Ts...>;
#endif

};

}

template<class F, class=void>
struct get_failure
{
    template<class... Ts>
    struct of
    {
#if FIT_REVEAL_USE_TEMPLATE_ALIAS
        template<class Id>
        using apply = decltype(Id()(std::declval<F>())(std::declval<Ts>()...));
#else
        template<class Id>
        static auto apply(Id id) -> decltype(id(std::declval<F>())(std::declval<Ts>()...));
#endif
    };
};

template<class F>
struct get_failure<F, typename std::enable_if<detail::has_failure<F>::value>::type>
: F::failure
{};

template<template<class...> class Template>
struct as_failure
{
    template<class... Ts>
    struct of
    {
#if FIT_REVEAL_USE_TEMPLATE_ALIAS
        template<class Id>
        using apply = typename Id::template defer<Template, Ts...>;
#else
        template<class Id>
        static auto apply(Id) -> decltype(Id::template defer<Template, Ts...>());
#endif
    };
};

namespace detail {
template<class Failure, class... Ts>
FIT_USING_TYPENAME(apply_failure, Failure::template of<Ts...>);

template<class F, class Failure>
struct reveal_failure
{
    // Add default constructor to make clang 3.4 happy
    constexpr reveal_failure()
    {}
    // This is just a placeholder to produce a note in the compiler, it is
    // never called
    template<
        class... Ts, 
        class=typename std::enable_if<(!is_callable<F, Ts...>::value)>::type
    >
    constexpr auto operator()(Ts&&... xs) const
#if FIT_REVEAL_USE_TEMPLATE_ALIAS
        -> typename apply_failure<Failure, Ts...>::template apply<fit::detail::identity_failure>;
#else
        -> decltype(apply_failure<Failure, Ts...>::apply(fit::detail::identity_failure()));
#endif
};

template<class F, class Failure=get_failure<F>, class=void>
struct traverse_failure 
: reveal_failure<F, Failure>
{
    constexpr traverse_failure()
    {}
};

template<class F, class Failure>
struct traverse_failure<F, Failure, typename holder< 
    typename Failure::children
>::type> 
: Failure::children::template overloads<F>
{
    constexpr traverse_failure()
    {}
};

template<class Failure, class Transform, class=void>
struct transform_failures 
: Transform::template apply<Failure>
{};

template<class Failure, class Transform>
struct transform_failures<Failure, Transform, typename holder< 
    typename Failure::children
>::type> 
: Failure::children::template transform<Transform>
{};

}

template<class Failure, class... Failures>
struct failures;

template<class... Fs>
struct with_failures
{
    typedef FIT_JOIN(failures, Fs...) children;
};

template<class Failure, class... Failures>
struct failures 
{
    template<class Transform>
    FIT_USING(transform, with_failures<detail::transform_failures<Failure, Transform>, detail::transform_failures<Failures, Transform>...>);

    template<class F, class FailureBase=FIT_JOIN(failures, Failures...)>
    struct overloads
    : detail::traverse_failure<F, Failure>, FailureBase::template overloads<F>
    {
        constexpr overloads()
        {}
        using detail::traverse_failure<F, Failure>::operator();
        using FailureBase::template overloads<F>::operator();
    };
};

template<class Failure>
struct failures<Failure>
{
    template<class Transform>
    FIT_USING(transform, with_failures<detail::transform_failures<Failure, Transform>>);

    template<class F>
    FIT_USING(overloads, detail::traverse_failure<F, Failure>);
};

template<class Transform, class... Fs>
struct failure_map
: with_failures<detail::transform_failures<get_failure<Fs>, Transform>...>
{};

template<class... Fs>
struct failure_for
: with_failures<get_failure<Fs>...>
{};

template<class F, class Base=detail::callable_base<F>>
struct reveal_adaptor
: detail::traverse_failure<Base>, Base
{
    typedef reveal_adaptor fit_rewritable1_tag;
    using detail::traverse_failure<Base>::operator();
    using Base::operator();

    FIT_INHERIT_CONSTRUCTOR(reveal_adaptor, Base);
};
// Avoid double reveals, it causes problem on gcc 4.6
template<class F>
struct reveal_adaptor<reveal_adaptor<F>>
: reveal_adaptor<F>
{
    typedef reveal_adaptor fit_rewritable1_tag;
    FIT_INHERIT_CONSTRUCTOR(reveal_adaptor, reveal_adaptor<F>);
};

FIT_DECLARE_STATIC_VAR(reveal, detail::make<reveal_adaptor>);

} // namespace fit

#endif
