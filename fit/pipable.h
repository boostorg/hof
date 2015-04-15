/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    pipable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_PIPABLE_H
#define FIT_GUARD_FUNCTION_PIPABLE_H

/// pipable
/// =======
/// 
/// Description
/// -----------
/// 
/// The `pipable` function adaptor provides an extension method. The first
/// parameter of the function can be piped into the function using the pipe
/// `|` operator. This can be especially convient when there are a lot of
/// nested function calls. Functions that are made pipable can still be called
/// the traditional way without piping in the first parameter.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr pipable_adaptor<F> pipable(F f);
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
///     FunctionObject
///     MoveConstructible
/// 
/// Example
/// -------
/// 
///     struct sum
///     {
///         template<class T, class U>
///         T operator()(T x, U y) const
///         {
///             return x+y;
///         }
///     };
/// 
///     assert(3 == 1 | pipable(sum())(2));
///     assert(3 == pipable(sum())(1, 2));
/// 

#include <fit/conditional.h>
#include <fit/static.h>
#include <fit/pack.h>
#include <fit/detail/delegate.h>
#include <fit/detail/move.h>
#include <fit/function.h>
#include <fit/detail/make.h>
#include <fit/detail/static_constexpr.h>

namespace fit { 
 
template<class F>
struct pipable_adaptor;

namespace detail {

template<class F, class Pack>
struct pipe_closure : F, Pack
{
    
    template<class X, class P>
    constexpr pipe_closure(X&& f, P&& pack) : F(fit::forward<X>(f)), Pack(fit::forward<P>(pack))
    {}

    template<class... Ts>
    constexpr const F& base_function(Ts&&...) const
    {
        return *this;
    }

    template<class... Ts>
    constexpr const Pack& get_pack(Ts&&...) const
    {
        return *this;
    }

    template<class A>
    struct invoke
    {
        A a;
        const pipe_closure * self;
        template<class X>
        constexpr invoke(X&& x, const pipe_closure * self) : a(fit::forward<X>(x)), self(self)
        {}

        FIT_RETURNS_CLASS(invoke);

        template<class... Ts>
        constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
        (FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->self->base_function(xs...))(fit::forward<A>(a), fit::forward<Ts>(xs)...));
    };

    FIT_RETURNS_CLASS(pipe_closure);

    template<class A>
    constexpr auto operator()(A&& a) const FIT_RETURNS
    (FIT_MANGLE_CAST(const Pack&)(FIT_CONST_THIS->get_pack(a))(invoke<A&&>(fit::forward<A>(a), FIT_CONST_THIS)));
};

template<class F, class Pack>
constexpr auto make_pipe_closure(F f, Pack&& p) FIT_RETURNS
(
    pipe_closure<F, typename std::remove_reference<Pack>::type>(fit::move(f), fit::forward<Pack>(p))
);


template<class Derived, class F>
struct pipe_pack
{
    template<class... Ts>
    constexpr const F& get_function(Ts&&...) const
    {
        return static_cast<const F&>(static_cast<const Derived&>(*this));
    }

    FIT_RETURNS_CLASS(pipe_pack);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (make_pipe_closure(FIT_RETURNS_C_CAST(F&&)(FIT_CONST_THIS->get_function(xs...)), fit::pack_forward(fit::forward<Ts>(xs)...)));
};
    
template<class A, class F, class Pack>
constexpr auto operator|(A&& a, const pipe_closure<F, Pack>& p) FIT_RETURNS
(p(fit::forward<A>(a)));

}

template<class F>
struct pipable_adaptor 
: conditional_adaptor<F, detail::pipe_pack<pipable_adaptor<F>, F> >
{
    typedef conditional_adaptor<F, detail::pipe_pack<pipable_adaptor<F>, F> > base;

    FIT_INHERIT_CONSTRUCTOR(pipable_adaptor, base);

    constexpr const F& base_function() const
    {
        return *this;
    }
};

template<class A, class F>
constexpr auto operator|(A&& a, const pipable_adaptor<F>& p) FIT_RETURNS
(p(fit::forward<A>(a)));

FIT_STATIC_CONSTEXPR detail::make<pipable_adaptor> pipable = {};

// Operators for static_ adaptor
template<class A, class F>
auto operator|(A&& a, static_<F> f) FIT_RETURNS
(f.base_function().base_function()(fit::forward<A>(a)));

// Operators for static_function_wrapper adaptor
template<class A, class F>
auto operator|(A&& a, const fit::detail::static_function_wrapper<F>& f) FIT_RETURNS
(f(fit::forward<A>(a)));
}

#endif
