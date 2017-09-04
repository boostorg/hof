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
/// * [ConstCallable](ConstCallable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
///     using namespace fit;
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
///     int main() {
///         assert(3 == (1 | pipable(sum())(2)));
///         assert(3 == pipable(sum())(1, 2));
///     }
/// 
/// References
/// ----------
/// 
/// * [Extension methods](<Extension methods>)
/// 

#include <fit/pack.hpp>
#include <fit/detail/builder.hpp>
#include <fit/detail/builder/partial.hpp>
#include <fit/indirect.hpp>
#include <fit/reveal.hpp>

namespace fit { 
 
namespace detail {

template<class F, class Pack>
struct pipe_closure : F, Pack
{
    
    template<class X, class P>
    constexpr pipe_closure(X&& fp, P&& packp) 
    FIT_NOEXCEPT(FIT_IS_NOTHROW_CONSTRUCTIBLE(F, X&&) && FIT_IS_NOTHROW_CONSTRUCTIBLE(Pack, P&&))
    : F(FIT_FORWARD(X)(fp)), Pack(FIT_FORWARD(P)(packp))
    {}

    template<class... Ts>
    constexpr const F& base_function(Ts&&...) const noexcept
    {
        return *this;
    }

    template<class... Ts>
    constexpr const Pack& get_pack(Ts&&...) const noexcept
    {
        return *this;
    }

    template<class A>
    struct invoke
    {
        A a;
        const pipe_closure * self;
        template<class X>
        constexpr invoke(X&& xp, const pipe_closure * selfp) 
        FIT_NOEXCEPT(FIT_IS_NOTHROW_CONSTRUCTIBLE(A, X&&))
        : a(FIT_FORWARD(X)(xp)), self(selfp)
        {}

        FIT_RETURNS_CLASS(invoke);

        template<class... Ts>
        constexpr FIT_SFINAE_RESULT(const F&, id_<A>, id_<Ts>...) 
        operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
        (FIT_RETURNS_STATIC_CAST(const F&)(*FIT_CONST_THIS->self)(FIT_FORWARD(A)(a), FIT_FORWARD(Ts)(xs)...));
    };

    FIT_RETURNS_CLASS(pipe_closure);

    template<class A>
    constexpr FIT_SFINAE_RESULT(const Pack&, id_<invoke<A&&>>) 
    operator()(A&& a) const FIT_SFINAE_RETURNS
    (FIT_MANGLE_CAST(const Pack&)(FIT_CONST_THIS->get_pack(a))(invoke<A&&>(FIT_FORWARD(A)(a), FIT_CONST_THIS)));
};

template<class F, class Pack>
constexpr auto make_pipe_closure(F f, Pack&& p) FIT_RETURNS
(
    pipe_closure<F, typename std::remove_reference<Pack>::type>(FIT_RETURNS_STATIC_CAST(F&&)(f), FIT_FORWARD(Pack)(p))
);

struct pipable_adaptor_builder
{
    template<class F>
    struct base
    {
        struct failure
        : failure_for<F>
        {};
    };

    struct apply
    {
        template<class F, class... Ts>
        constexpr auto operator()(F&& f, Ts&&... xs) const 
        FIT_RETURNS
        (
            fit::detail::make_pipe_closure(
                // FIT_FORWARD(F)(f), 
                fit::indirect(&f), 
                fit::pack_forward(FIT_FORWARD(Ts)(xs)...)
            )
        );
    };
};
    
template<class A, class F, class Pack>
constexpr auto operator|(A&& a, const pipe_closure<F, Pack>& p) FIT_RETURNS
(p(FIT_FORWARD(A)(a)));

}

FIT_DECLARE_ADAPTOR(pipable, detail::partial_adaptor_builder<detail::pipable_adaptor_builder>)

template<class A, class F>
constexpr auto operator|(A&& a, const pipable_adaptor<F>& p) FIT_RETURNS
(p(FIT_FORWARD(A)(a)));

namespace detail {

template<class F>
struct static_function_wrapper;

// Operators for static_function_wrapper adaptor
template<class A, class F>
auto operator|(A&& a, const fit::detail::static_function_wrapper<F>& f) FIT_RETURNS
(f(FIT_FORWARD(A)(a)));

}

template<class F>
struct static_;

// Operators for static_ adaptor
template<class A, class F>
auto operator|(A&& a, static_<F> f) FIT_RETURNS
(f.base_function().base_function()(FIT_FORWARD(A)(a)));

} // namespace fit

#endif
