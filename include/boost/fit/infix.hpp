/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    infix.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FUNCTION_INFIX_H
#define BOOST_FIT_GUARD_FUNCTION_INFIX_H

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
///     using namespace boost::fit;
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

#include <boost/fit/detail/delegate.hpp>
#include <boost/fit/detail/callable_base.hpp>
#include <boost/fit/always.hpp>
#include <boost/fit/reveal.hpp>
#include <boost/fit/detail/move.hpp>
#include <boost/fit/detail/make.hpp>
#include <boost/fit/detail/static_const_var.hpp>

namespace boost { namespace fit {
 
namespace detail{
template<class T, class F>
struct postfix_adaptor : F
{
    T x;

    template<class X, class XF>
    constexpr postfix_adaptor(X&& xp, XF&& fp)
    BOOST_FIT_NOEXCEPT(BOOST_FIT_IS_NOTHROW_CONSTRUCTIBLE(F, XF&&) && BOOST_FIT_IS_NOTHROW_CONSTRUCTIBLE(T, X&&)) 
    : F(BOOST_FIT_FORWARD(XF)(fp)), x(BOOST_FIT_FORWARD(X)(xp))
    {}

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const noexcept
    {
        return boost::fit::always_ref(*this)(xs...);
    }

    BOOST_FIT_RETURNS_CLASS(postfix_adaptor);

    template<class... Ts>
    constexpr BOOST_FIT_SFINAE_RESULT(const F&, id_<T&&>, id_<Ts>...)
    operator()(Ts&&... xs) const BOOST_FIT_SFINAE_RETURNS
    (
        (BOOST_FIT_MANGLE_CAST(const F&)(BOOST_FIT_CONST_THIS->base_function(xs...)))(BOOST_FIT_RETURNS_C_CAST(T&&)(BOOST_FIT_CONST_THIS->x), BOOST_FIT_FORWARD(Ts)(xs)...)
    );

    template<class A>
    constexpr BOOST_FIT_SFINAE_RESULT(const F&, id_<T&&>, id_<A>)
    operator>(A&& a) const BOOST_FIT_SFINAE_RETURNS
    (
        (BOOST_FIT_MANGLE_CAST(const F&)(BOOST_FIT_CONST_THIS->base_function(a)))(BOOST_FIT_RETURNS_C_CAST(T&&)(BOOST_FIT_CONST_THIS->x), BOOST_FIT_FORWARD(A)(a))
    );
};

template<class T, class F>
constexpr postfix_adaptor<T, F> make_postfix_adaptor(T&& x, F f)
BOOST_FIT_NOEXCEPT_CONSTRUCTIBLE(postfix_adaptor<T, F>, T&&, F&&)
{
    return postfix_adaptor<T, F>(BOOST_FIT_FORWARD(T)(x), static_cast<F&&>(f));
}
}

template<class F>
struct infix_adaptor : detail::callable_base<F>
{
    typedef infix_adaptor fit_rewritable1_tag;
    BOOST_FIT_INHERIT_CONSTRUCTOR(infix_adaptor, detail::callable_base<F>);

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const noexcept
    {
        return boost::fit::always_ref(*this)(xs...);
    }

    template<class... Ts>
    constexpr const detail::callable_base<F>& infix_base_function(Ts&&... xs) const noexcept
    {
        return boost::fit::always_ref(*this)(xs...);
    }

    BOOST_FIT_RETURNS_CLASS(infix_adaptor);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const BOOST_FIT_RETURNS
    (
        (BOOST_FIT_MANGLE_CAST(const detail::callable_base<F>&)(BOOST_FIT_CONST_THIS->base_function(xs...)))(BOOST_FIT_FORWARD(Ts)(xs)...)
    );
};

template<class T, class F>
constexpr auto operator<(T&& x, const infix_adaptor<F>& i) BOOST_FIT_RETURNS
(detail::make_postfix_adaptor(BOOST_FIT_FORWARD(T)(x), boost::fit::move(i.base_function(x))));

// TODO: Operators for static_

namespace detail {

template<class F>
struct static_function_wrapper;

// Operators for static_function_wrapper adaptor
template<class T, class F>
auto operator<(T&& x, const boost::fit::detail::static_function_wrapper<F>& f) BOOST_FIT_RETURNS
(
    detail::make_postfix_adaptor(BOOST_FIT_FORWARD(T)(x), boost::fit::move(f.base_function().infix_base_function()))
);

template<class F>
struct static_default_function;

// Operators for static_default_function adaptor
template<class T, class F>
auto operator<(T&& x, const boost::fit::detail::static_default_function<F>&) BOOST_FIT_RETURNS
(
    detail::make_postfix_adaptor(BOOST_FIT_FORWARD(T)(x), boost::fit::move(F().infix_base_function()))
);
}
// This overload is needed for gcc
template<class T, class F>
constexpr auto operator<(T&& x, const boost::fit::reveal_adaptor<F>& f) BOOST_FIT_RETURNS
(
    detail::make_postfix_adaptor(BOOST_FIT_FORWARD(T)(x), f.infix_base_function())
);

BOOST_FIT_DECLARE_STATIC_VAR(infix, detail::make<infix_adaptor>);

}} // namespace boost::fit

#endif
