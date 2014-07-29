/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    placeholders.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_PLACEHOLDERS_H
#define FIT_GUARD_FUNCTION_PLACEHOLDERS_H

#include <fit/lazy.h>

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

FIT_FOREACH_BINARY_OP(FIT_PLACEHOLDER_BINARY_OP)

constexpr const placeholder<1> _1 = {};
constexpr const placeholder<2> _2 = {};
constexpr const placeholder<3> _3 = {};
constexpr const placeholder<4> _4 = {};
constexpr const placeholder<5> _5 = {};
constexpr const placeholder<6> _6 = {};
constexpr const placeholder<7> _7 = {};
constexpr const placeholder<8> _8 = {};
constexpr const placeholder<9> _9 = {};

}

namespace std {
    template<int N>
    struct is_placeholder<fit::placeholder<N>>
    : std::integral_constant<int, N>
    {};
}

#endif