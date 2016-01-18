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
/// 
/// Requirements
/// ------------
/// 
/// Projection must be:
/// 
/// * [UnaryCallable](concepts.md#unarycallable)
/// * MoveConstructible
/// 
/// F must be:
/// 
/// * [Callable](concepts.md#callable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     struct foo
///     {
///         foo(int x) : x(x)
///         {}
///         int x;
///     };
///     assert(fit::by(&foo::x, _ + _)(foo(1), foo(2)) == 3);
/// 



#include <utility>
#include <fit/always.hpp>
#include <fit/detail/callable_base.hpp>
#include <fit/detail/result_of.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/static_const_var.hpp>
#include <fit/apply_eval.hpp>

namespace fit {

namespace detail {

template<class T, class Projection>
struct project_eval
{
    T&& x;
    const Projection& p;

    template<class X, class P>
    constexpr project_eval(X&& xp, const P& pp) : x(fit::forward<X>(xp)), p(pp)
    {}

    constexpr auto operator()() const FIT_RETURNS
    (p(fit::forward<T>(x)));
};

template<class T, class Projection>
constexpr project_eval<T, Projection> make_project_eval(T&& x, const Projection& p)
{
    return project_eval<T, Projection>(fit::forward<T>(x), p);
}

template<class T, class Projection>
struct project_void_eval
{
    T&& x;
    const Projection& p;

    template<class X, class P>
    constexpr project_void_eval(X&& xp, const P& pp) : x(fit::forward<X>(xp)), p(pp)
    {}

    struct void_ {};

    constexpr void_ operator()() const
    {
        return p(fit::forward<T>(x)), void_();
    }
};

template<class T, class Projection>
constexpr project_void_eval<T, Projection> make_project_void_eval(T&& x, const Projection& p)
{
    return project_void_eval<T, Projection>(fit::forward<T>(x), p);
}

template<class Projection, class F, class... Ts, 
    class R=decltype(
        std::declval<const F&>()(std::declval<const Projection&>()(std::declval<Ts>())...)
    )>
constexpr R by_eval(const Projection& p, const F& f, Ts&&... xs)
{
    return apply_eval(f, make_project_eval(fit::forward<Ts>(xs), p)...);
}

#if FIT_NO_ORDERD_BRACE_INIT
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
    return apply_eval(always(), make_project_void_eval(fit::forward<Ts>(xs), p)...);
}

struct swallow
{
    template<class... Ts>
    constexpr swallow(Ts&&...)
    {}
};

}

template<class Projection, class F=void>
struct by_adaptor;

template<class Projection, class F>
struct by_adaptor : detail::callable_base<Projection>, detail::callable_base<F>
{
    typedef by_adaptor fit_rewritable_tag;
    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Ts>
    constexpr const detail::callable_base<Projection>& base_projection(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    struct by_failure
    {
        template<class Failure>
        struct apply
        {
            template<class... Ts>
            struct of
            : Failure::template of<decltype(std::declval<detail::callable_base<Projection>>()(std::declval<Ts>()))...>
            {};
        };
    };

    struct failure
    : failure_map<by_failure, detail::callable_base<F>>
    {};

    FIT_INHERIT_DEFAULT(by_adaptor, detail::callable_base<Projection>, F)

    template<class P, class G, FIT_ENABLE_IF_CONVERTIBLE(P, detail::callable_base<Projection>), FIT_ENABLE_IF_CONVERTIBLE(G, detail::callable_base<F>)>
    constexpr by_adaptor(P&& p, G&& f) 
    : detail::callable_base<Projection>(fit::forward<P>(p)), detail::callable_base<F>(fit::forward<G>(f))
    {}

    FIT_RETURNS_CLASS(by_adaptor);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(const detail::callable_base<F>&, result_of<const detail::callable_base<Projection>&, id_<Ts>>...) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        detail::by_eval(
            FIT_MANGLE_CAST(const detail::callable_base<Projection>&)(FIT_CONST_THIS->base_projection(xs...)),
            FIT_MANGLE_CAST(const detail::callable_base<F>&)(FIT_CONST_THIS->base_function(xs...)),
            fit::forward<Ts>(xs)...
        )
    );
};

template<class Projection>
struct by_adaptor<Projection, void> : detail::callable_base<Projection>
{
    typedef by_adaptor fit_rewritable1_tag;
    template<class... Ts>
    constexpr const detail::callable_base<Projection>& base_projection(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_INHERIT_DEFAULT(by_adaptor, detail::callable_base<Projection>)

    template<class P, FIT_ENABLE_IF_CONVERTIBLE(P, detail::callable_base<Projection>)>
    constexpr by_adaptor(P&& p) 
    : detail::callable_base<Projection>(fit::forward<P>(p))
    {}

    FIT_RETURNS_CLASS(by_adaptor);

    template<class... Ts>
    constexpr FIT_BY_VOID_RETURN operator()(Ts&&... xs) const
    {
#if FIT_NO_ORDERD_BRACE_INIT
        return detail::by_void_eval(this->base_projection(xs...), fit::forward<Ts>(xs)...);
#else
#if FIT_NO_CONSTEXPR_VOID
        return
#endif
        detail::swallow{
            (this->base_projection(xs...)(fit::forward<Ts>(xs)), 0)...
        };
#endif
    }
};

FIT_DECLARE_STATIC_VAR(by, detail::make<by_adaptor>);

} // namespace fit
#endif
