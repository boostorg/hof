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
/// `|` operator. This can be especially convenient when there are a lot of
/// nested function calls. Functions that are made pipable can still be called
/// the traditional way without piping in the first parameter.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr pipable_adaptor<F> pipable(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(x | pipable(f)(ys...) == f(x, ys...));
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [Callable](concepts.md#callable)
/// * MoveConstructible
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

#include <fit/conditional.hpp>
#include <fit/pack.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/static_const_var.hpp>
#include <fit/limit.hpp>

namespace fit { 
 
template<class F>
struct pipable_adaptor;

namespace detail {

template<class F, class Pack>
struct pipe_closure : F, Pack
{
    
    template<class X, class P>
    constexpr pipe_closure(X&& fp, P&& packp) : F(fit::forward<X>(fp)), Pack(fit::forward<P>(packp))
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
        constexpr invoke(X&& xp, const pipe_closure * selfp) : a(fit::forward<X>(xp)), self(selfp)
        {}

        FIT_RETURNS_CLASS(invoke);

        template<class... Ts>
        constexpr FIT_SFINAE_RESULT(const F&, id_<A>, id_<Ts>...) 
        operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
        (FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->self->base_function(xs...))(fit::forward<A>(a), fit::forward<Ts>(xs)...));
    };

    FIT_RETURNS_CLASS(pipe_closure);

    template<class A>
    constexpr FIT_SFINAE_RESULT(const Pack&, id_<invoke<A&&>>) 
    operator()(A&& a) const FIT_SFINAE_RETURNS
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

    template<class... Ts, class=typename std::enable_if<
        sizeof...(Ts) < function_param_limit<F>::value
    >::type>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (make_pipe_closure(FIT_RETURNS_C_CAST(F&&)(FIT_CONST_THIS->get_function(xs...)), fit::pack_forward(fit::forward<Ts>(xs)...)));
};
    
template<class A, class F, class Pack>
constexpr auto operator|(A&& a, const pipe_closure<F, Pack>& p) FIT_RETURNS
(p(fit::forward<A>(a)));

}

template<class F>
struct pipable_adaptor 
: conditional_adaptor<detail::callable_base<F>, detail::pipe_pack<pipable_adaptor<F>, detail::callable_base<F>> >
{
    typedef conditional_adaptor<detail::callable_base<F>, detail::pipe_pack<pipable_adaptor<F>, detail::callable_base<F>> > base;
    typedef pipable_adaptor fit_rewritable_tag;

    FIT_INHERIT_CONSTRUCTOR(pipable_adaptor, base);

    constexpr const detail::callable_base<F>& base_function() const
    {
        return *this;
    }
};

template<class A, class F>
constexpr auto operator|(A&& a, const pipable_adaptor<F>& p) FIT_RETURNS
(p(fit::forward<A>(a)));

FIT_DECLARE_STATIC_VAR(pipable, detail::make<pipable_adaptor>);

namespace detail {

template<class F>
struct static_function_wrapper;

// Operators for static_function_wrapper adaptor
template<class A, class F>
auto operator|(A&& a, const fit::detail::static_function_wrapper<F>& f) FIT_RETURNS
(f(fit::forward<A>(a)));

template<class F>
struct static_default_function;

// Operators for static_default_function adaptor
template<class A, class F>
auto operator|(A&& a, const fit::detail::static_default_function<F>& f) FIT_RETURNS
(f(fit::forward<A>(a)));

}

template<class F>
struct static_;

// Operators for static_ adaptor
template<class A, class F>
auto operator|(A&& a, static_<F> f) FIT_RETURNS
(f.base_function().base_function()(fit::forward<A>(a)));

} // namespace fit

#endif
