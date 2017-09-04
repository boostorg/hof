/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    infix.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_INFIX_H
#define FIT_GUARD_FUNCTION_INFIX_H

/// infix
/// =====
/// 
/// Description
/// -----------
/// 
/// The `infix` function adaptor allows the function to be used as an infix
/// operator. The operator must be placed inside the angle brackets(ie `<`
/// and `>`).
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr infix_adaptor<F> infix(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(x <infix(f)> y == f(x, y));
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [BinaryCallable](BinaryCallable)
/// * MoveConstructible
/// 
/// Operator precedence
/// -------------------
/// 
/// Infix operators have the precedence of relational operators. This means
/// operators such as `+` or `*` have higher precedence:
/// 
///     assert((x + y <infix(f)> z) == ((x + y) <infix(f)> z));
///     assert((x * y <infix(f)> z) == ((x * y) <infix(f)> z));
/// 
/// However, operators such as `|` or `==` have lower precedence::
/// 
///     assert((x | y <infix(f)> z) == (x | (y <infix(f)> z)));
///     assert((x == y <infix(f)> z) == (x == (y <infix(f)> z)));
/// 
/// Also, infix operators have left-to-right associativity:
/// 
///     assert(x <infix(f)> y <infix(g)> z == ((x <infix(f)> y) <infix(g)> z));
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
///     using namespace fit;
/// 
///     struct plus_f
///     {
///         template<class T, class U>
///         T operator()(T x, U y) const
///         {
///             return x+y;
///         }
///     };
///     
///     int main() {
///         constexpr infix_adaptor<plus_f> plus = {};
///         int r = 3 <plus> 2;
///         assert(r == 5);
///     }
/// 

#include <fit/detail/builder.hpp>
#include <fit/detail/builder/unary.hpp>
#include <fit/reveal.hpp>

namespace fit {

template<class... Ts>
struct infix_adaptor;
 
namespace detail{
template<class T, class F>
struct postfix_adaptor : F
{
    T x;

    template<class X, class XF>
    constexpr postfix_adaptor(X&& xp, XF&& fp)
    FIT_NOEXCEPT(FIT_IS_NOTHROW_CONSTRUCTIBLE(F, XF&&) && FIT_IS_NOTHROW_CONSTRUCTIBLE(T, X&&)) 
    : F(FIT_FORWARD(XF)(fp)), x(FIT_FORWARD(X)(xp))
    {}

    template<class A>
    friend constexpr auto operator>(postfix_adaptor f, A&& a) FIT_RETURNS
    (
        f(fit::move(always_ref(f)(a).x), FIT_FORWARD(A)(a))
    );
};

template<class T, class F>
constexpr postfix_adaptor<T, F> make_postfix_adaptor(T&& x, F f)
FIT_NOEXCEPT_CONSTRUCTIBLE(postfix_adaptor<T, F>, T&&, F&&)
{
    return postfix_adaptor<T, F>(FIT_FORWARD(T)(x), fit::move(f));
}

// template<class T, class F>
// constexpr auto make_postfix_adaptor(T&& x, const infix_adaptor<F>& f) FIT_RETURNS
// (
//     fit::detail::make_postfix_adaptor(FIT_FORWARD(T)(x), f.base_function())
// );

struct infix_adaptor_builder
{
    template<class F>
    struct base
    {
        struct infix_failure
        {
            template<class Failure>
            struct apply
            {
                template<class... Ts>
                struct of
                : Failure::template of<Ts...>
                {};
            };
        };

        struct failure
        : failure_map<infix_failure, F>
        {};
    };

    struct apply
    {
        template<class F, class... Ts>
        constexpr FIT_SFINAE_RESULT(F&&, id_<Ts>...) 
        operator()(F&& f, Ts&&... xs) const FIT_SFINAE_RETURNS
        (
            FIT_FORWARD(F)(f)(FIT_FORWARD(Ts)(xs)...)
        );
    };

};
}

FIT_DECLARE_ADAPTOR(infix, detail::unary_adaptor_builder<detail::infix_adaptor_builder>)

template<class T, class F>
constexpr auto operator<(T&& x, infix_adaptor<F> i) FIT_RETURNS
(detail::make_postfix_adaptor(FIT_FORWARD(T)(x), fit::move(i.base_function(x))));

// TODO: Operators for static_

namespace detail {

template<class F>
struct static_function_wrapper;

// Operators for static_function_wrapper adaptor
template<class T, class F>
auto operator<(T&& x, const fit::detail::static_function_wrapper<F>& f) FIT_RETURNS
(
    fit::detail::make_postfix_adaptor(FIT_FORWARD(T)(x), fit::move(f.base_function()))
);
}
// This overload is needed for gcc
template<class T, class F>
constexpr auto operator<(T&& x, const fit::reveal_adaptor<F>& f) FIT_RETURNS
(
    fit::detail::make_postfix_adaptor(FIT_FORWARD(T)(x), f.base_function())
);

} // namespace fit

#endif
