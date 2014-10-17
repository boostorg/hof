/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    placeholders.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_PLACEHOLDERS_H
#define FIT_GUARD_FUNCTION_PLACEHOLDERS_H

/// placeholders
/// ============
/// 
/// Description
/// -----------
/// 
/// The placeholders provide `std::bind` compatible placeholders, that
/// additionally provide basic C++ operators that creates bind expressions.
/// Each bind expression supports `constexpr` function evaluation.
/// 
/// Synopsis
/// --------
/// 
///     placeholder<1> _1 = {};
///     placeholder<2> _2 = {};
///     placeholder<3> _3 = {};
///     placeholder<4> _4 = {};
///     placeholder<5> _5 = {};
///     placeholder<6> _6 = {};
///     placeholder<7> _7 = {};
///     placeholder<8> _8 = {};
///     placeholder<9> _9 = {};
/// 
/// 
/// Example
/// -------
/// 
///     auto sum = _1 + _2;
///     assert(3 == sum(1, 2));
/// 
/// 
/// unamed placeholder
/// ==================
/// 
/// Description
/// -----------
/// 
/// The unamed placeholder can be used to build simple functions from C++
/// operators. 
/// 
/// Note: The function produced by the unamed placeholder is not a bind expression.
/// 
/// Synopsis
/// --------
/// 
///     /* unspecified */ _ = {};
/// 
/// Example
/// -------
/// 
///     auto sum = _ + _;
///     assert(3 == sum(1, 2));
/// 

#include <fit/lazy.h>
#include <fit/protect.h>

namespace fit { namespace detail {
    template<int N>
    struct simple_placeholder
    {};
}}

namespace std {
    template<int N>
    struct is_placeholder<fit::detail::simple_placeholder<N>>
    : std::integral_constant<int, N>
    {};
}


namespace fit {

#define FIT_FOREACH_BINARY_OP(m) \
    m(+, add) \
    m(-, subtract) \
    m(*, multiply) \
    m(/, divide) \
    m(%, remainder) \
    m(>>, shift_right) \
    m(<<, shift_left) \
    m(>, greater_than) \
    m(<, less_than) \
    m(<=, less_than_equal) \
    m(>=, greater_than_equal) \
    m(==, equal) \
    m(!=, not_equal) \
    m(&, bit_and) \
    m(^, xor_) \
    m(|, bit_or) \
    m(&&, and_) \
    m(||, or_)

#define FIT_FOREACH_ASSIGN_OP(m) \
    m(+=, assign_add) \
    m(-=, assign_subtract) \
    m(*=, assign_multiply) \
    m(/=, assign_divide) \
    m(%=, assign_remainder) \
    m(>>=, assign_right_shift) \
    m(<<=, assign_left_shift) \
    m(&=, assign_bit_and) \
    m(|=, assign_bit_or) \
    m(^=, assign_xor)

#define FIT_FOREACH_UNARY_OP(m) \
    m(!, not_) \
    m(~, compl_) \
    m(+, unary_plus) \
    m(-, unary_subtract) \
    m(*, dereference) \
    m(++, increment) \
    m(--, decrement)

namespace operators {
#define FIT_BINARY_OP(op, name) \
    struct name \
    { \
        template<class T, class U> \
        constexpr auto operator()(T&& x, U&& y) const FIT_RETURNS \
        (std::forward<T>(x) op std::forward<U>(y)); \
    };

FIT_FOREACH_BINARY_OP(FIT_BINARY_OP)
FIT_FOREACH_ASSIGN_OP(FIT_BINARY_OP)

#define FIT_UNARY_OP(op, name) \
    struct name \
    { \
        template<class T> \
        constexpr auto operator()(T&& x) const FIT_RETURNS \
        (op(std::forward<T>(x))); \
    };


FIT_FOREACH_UNARY_OP(FIT_UNARY_OP)

}

template<int N>
struct placeholder
{
#define FIT_PLACEHOLDER_UNARY_OP(op, name) \
    constexpr auto operator op () const FIT_RETURNS \
    ( fit::lazy(operators::name())(detail::simple_placeholder<N>()) );

FIT_FOREACH_UNARY_OP(FIT_PLACEHOLDER_UNARY_OP)

#define FIT_PLACEHOLDER_ASSIGN_OP(op, name) \
    template<class T> \
    constexpr auto operator op (T&& x) const FIT_RETURNS \
    ( fit::lazy(operators::name())(detail::simple_placeholder<N>(), std::forward<T>(x)) );

FIT_FOREACH_ASSIGN_OP(FIT_PLACEHOLDER_ASSIGN_OP)

};

#if FIT_HAS_MANGLE_OVERLOAD

#define FIT_PLACEHOLDER_BINARY_OP(op, name) \
    template<class T, int N> \
    constexpr inline auto operator op (const placeholder<N>&, T&& x) FIT_RETURNS \
    ( fit::lazy(operators::name())(detail::simple_placeholder<N>(), std::forward<T>(x)) ); \
    template<class T, int N> \
    constexpr inline auto operator op (T&& x, const placeholder<N>&) FIT_RETURNS \
    ( fit::lazy(operators::name())(std::forward<T>(x), detail::simple_placeholder<N>()) ); \
    template<int N, int M> \
    constexpr inline auto operator op (const placeholder<N>&, const placeholder<M>&) FIT_RETURNS \
    ( fit::lazy(operators::name())(detail::simple_placeholder<N>(), detail::simple_placeholder<M>()) );

#else

#define FIT_PLACEHOLDER_BINARY_OP(op, name) \
    template<class T, class U> \
    struct result_ ## name \
    { typedef decltype(fit::lazy(operators::name())(std::declval<T>(), std::declval<U>())) type; }; \
    template<class T, int N> \
    constexpr inline typename result_ ## name<detail::simple_placeholder<N>, T>::type operator op (const placeholder<N>&, T&& x) \
    { return fit::lazy(operators::name())(detail::simple_placeholder<N>(), std::forward<T>(x)); } \
    template<class T, int N> \
    constexpr inline typename result_ ## name<T, detail::simple_placeholder<N>>::type operator op (T&& x, const placeholder<N>&) \
    { return fit::lazy(operators::name())(std::forward<T>(x), detail::simple_placeholder<N>()); } \
    template<int N, int M> \
    constexpr inline typename result_ ## name<detail::simple_placeholder<N>, detail::simple_placeholder<M>>::type operator op (const placeholder<N>&, const placeholder<M>&) \
    { return fit::lazy(operators::name())(detail::simple_placeholder<N>(), detail::simple_placeholder<M>()); }

#endif

FIT_FOREACH_BINARY_OP(FIT_PLACEHOLDER_BINARY_OP)

static constexpr placeholder<1> _1 = {};
static constexpr placeholder<2> _2 = {};
static constexpr placeholder<3> _3 = {};
static constexpr placeholder<4> _4 = {};
static constexpr placeholder<5> _5 = {};
static constexpr placeholder<6> _6 = {};
static constexpr placeholder<7> _7 = {};
static constexpr placeholder<8> _8 = {};
static constexpr placeholder<9> _9 = {};


namespace detail {



struct unamed_placeholder
{
template<class T, class Invoker>
struct partial_ap
{
    T val;
    partial_ap(const T& x) : val(x)
    {}

    FIT_RETURNS_CLASS(partial_ap);

    template<class X>
    auto operator()(X&& x) const FIT_RETURNS
    (Invoker()(FIT_CONST_THIS->val, std::forward<X>(x)));
};

template<class Op>
struct left
{
    template<class T, class X>
    auto operator()(const T& val, X&& x) const FIT_RETURNS
    (Op()(val, std::forward<X>(x)));
};

template<class Op>
struct right
{
    template<class T, class X>
    auto operator()(const T& val, X&& x) const FIT_RETURNS
    (Op()(std::forward<X>(x), val));
};

#define FIT_UNAMED_PLACEHOLDER_UNARY_OP(op, name) \
    constexpr auto operator op () const FIT_RETURNS \
    ( operators::name() );

FIT_FOREACH_UNARY_OP(FIT_UNAMED_PLACEHOLDER_UNARY_OP)

#define FIT_UNAMED_PLACEHOLDER_ASSIGN_OP(op, name) \
    template<class T> \
    constexpr auto operator op (const T& x) const FIT_RETURNS \
    ( partial_ap<T, left<operators::name>>(x) );

FIT_FOREACH_ASSIGN_OP(FIT_UNAMED_PLACEHOLDER_ASSIGN_OP)
};
#define FIT_UNAMED_PLACEHOLDER_BINARY_OP(op, name) \
    template<class T> \
    constexpr inline auto operator op (const unamed_placeholder&, const T& x) FIT_RETURNS \
    ( unamed_placeholder::partial_ap<T, unamed_placeholder::left<operators::name>>(x) ); \
    template<class T> \
    constexpr inline auto operator op (const T& x, const unamed_placeholder&) FIT_RETURNS \
    ( unamed_placeholder::partial_ap<T, unamed_placeholder::right<operators::name>>(x) ); \
    constexpr inline auto operator op (const unamed_placeholder&, const unamed_placeholder&) FIT_RETURNS \
    ( operators::name() );

FIT_FOREACH_BINARY_OP(FIT_UNAMED_PLACEHOLDER_BINARY_OP)
}

static constexpr detail::unamed_placeholder _ = {};

}

namespace std {
    template<int N>
    struct is_placeholder<fit::placeholder<N>>
    : std::integral_constant<int, N>
    {};
}

#endif