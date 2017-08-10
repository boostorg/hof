/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    by.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_ON_H
#define FIT_GUARD_FUNCTION_ON_H

/// by
/// ==
/// 
/// Description
/// -----------
/// 
/// The `by` function adaptor applies a projection onto the parameters of
/// another function. This is useful, for example, to define a function for
/// sorting such that the ordering is based off of the value of one of its
/// member fields. 
/// 
/// Also, if just a projection is given, then the projection will be called
/// for each of its arguments.
/// 
/// Note: All projections are always evaluated in order from left-to-right.
/// 
/// Synopsis
/// --------
/// 
///     template<class Projection, class F>
///     constexpr by_adaptor<Projection, F> by(Projection p, F f);
/// 
///     template<class Projection>
///     constexpr by_adaptor<Projection> by(Projection p);
/// 
/// Semantics
/// ---------
/// 
///     assert(by(p, f)(xs...) == f(p(xs)...));
///     assert(by(p)(xs...) == p(xs)...);
/// 
/// Requirements
/// ------------
/// 
/// Projection must be:
/// 
/// * [UnaryCallable](UnaryCallable)
/// * MoveConstructible
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
///     struct foo
///     {
///         foo(int x_) : x(x_)
///         {}
///         int x;
///     };
/// 
///     int main() {
///         assert(fit::by(&foo::x, _ + _)(foo(1), foo(2)) == 3);
///     }
/// 
/// References
/// ----------
/// 
/// * [Projections](Projections)
/// * [Variadic print](<Variadic print>)
/// 

#include <fit/detail/builder.hpp>
#include <fit/detail/builder/binary.hpp>
#include <fit/apply_eval.hpp>
#include <fit/reveal.hpp>

namespace fit {

namespace detail {

template<class T, class Projection>
struct project_eval
{
    T&& x;
    const Projection& p;

    template<class X, class P>
    constexpr project_eval(X&& xp, const P& pp) : x(FIT_FORWARD(X)(xp)), p(pp)
    {}

    constexpr auto operator()() const FIT_RETURNS
    (p(FIT_FORWARD(T)(x)));
};

template<class T, class Projection>
constexpr project_eval<T, Projection> make_project_eval(T&& x, const Projection& p)
{
    return project_eval<T, Projection>(FIT_FORWARD(T)(x), p);
}

template<class T, class Projection>
struct project_void_eval
{
    T&& x;
    const Projection& p;

    template<class X, class P>
    constexpr project_void_eval(X&& xp, const P& pp) : x(FIT_FORWARD(X)(xp)), p(pp)
    {}

    struct void_ {};

    constexpr void_ operator()() const
    {
        return p(FIT_FORWARD(T)(x)), void_();
    }
};

template<class T, class Projection>
constexpr project_void_eval<T, Projection> make_project_void_eval(T&& x, const Projection& p)
{
    return project_void_eval<T, Projection>(FIT_FORWARD(T)(x), p);
}

template<class Projection, class F, class... Ts, 
    class R=decltype(
        std::declval<const F&>()(std::declval<const Projection&>()(std::declval<Ts>())...)
    )>
constexpr R by_eval(const Projection& p, const F& f, Ts&&... xs)
{
    return fit::apply_eval(f, make_project_eval(FIT_FORWARD(Ts)(xs), p)...);
}

#if FIT_NO_ORDERED_BRACE_INIT
#define FIT_BY_VOID_RETURN FIT_ALWAYS_VOID_RETURN
#else
#if FIT_NO_CONSTEXPR_VOID
#define FIT_BY_VOID_RETURN fit::detail::swallow
#else
#define FIT_BY_VOID_RETURN void
#endif
#endif

template<class Projection, class... Ts>
constexpr FIT_ALWAYS_VOID_RETURN by_void_eval(const Projection& p, Ts&&... xs)
{
    return fit::apply_eval(fit::always(), detail::make_project_void_eval(FIT_FORWARD(Ts)(xs), p)...);
}

struct swallow
{
    template<class... Ts>
    constexpr swallow(Ts&&...)
    {}
};

struct by_binary_adaptor_base
{
    template<class Projection, class F>
    struct base
    {
        struct by_failure
        {
            template<class Failure>
            struct apply
            {
                template<class... Ts>
                struct of
                : Failure::template of<decltype(std::declval<Projection>()(std::declval<Ts>()))...>
                {};
            };
        };

        struct failure
        : failure_map<by_failure, F>
        {};
    };

    struct apply
    {
        template<class Projection, class F, class... Ts>
        constexpr FIT_SFINAE_RESULT(F&&, result_of<Projection&&, id_<Ts>>...) 
        operator()(Projection&& p, F&& f, Ts&&... xs) const FIT_SFINAE_RETURNS
        (
            detail::by_eval(
                FIT_FORWARD(Projection)(p),
                FIT_FORWARD(F)(f),
                FIT_FORWARD(Ts)(xs)...
            )
        );
    };
};

struct by_unary_adaptor_base
{
    template<class F>
    struct base
    {};

    struct apply
    {
        template<class Projection, class... Ts, class=detail::holder<decltype(std::declval<Projection>()(std::declval<Ts>()))...>>
        constexpr FIT_BY_VOID_RETURN operator()(Projection&& p, Ts&&... xs) const 
        {
#if FIT_NO_ORDERED_BRACE_INIT
            return detail::by_void_eval(FIT_FORWARD(Projection)(p), FIT_FORWARD(Ts)(xs)...);
#else
#if FIT_NO_CONSTEXPR_VOID
            return
#endif
            detail::swallow{
                (FIT_FORWARD(Projection)(p)(FIT_FORWARD(Ts)(xs)), 0)...
            };
#endif
        }
    };

};

}

FIT_DECLARE_ADAPTOR(by, detail::binary_adaptor_builder<detail::by_binary_adaptor_base, detail::by_unary_adaptor_base>)

} // namespace fit
#endif
