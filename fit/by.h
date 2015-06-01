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
/// Requirements
/// ------------
/// 
/// Projection must be:
/// 
///     UnaryFunctionObject
///     MoveConstructible
/// 
/// F must be:
/// 
///     FunctionObject
///     MoveConstructible
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
///     assert(fit::by(std::mem_fn(&foo::x), _ + _)(foo(1), foo(2)) == 3);
/// 



#include <utility>
#include <fit/always.h>
#include <fit/detail/delegate.h>
#include <fit/detail/result_of.h>
#include <fit/detail/move.h>
#include <fit/detail/make.h>
#include <fit/detail/static_const_var.h>
#include <fit/apply_eval.h>

namespace fit {

namespace detail {

template<class T, class Projection>
struct project_eval
{
    T&& x;
    const Projection& p;

    template<class X, class P>
    constexpr project_eval(X&& x, const P& p) : x(fit::forward<X>(x)), p(p)
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
    constexpr project_void_eval(X&& x, const P& p) : x(fit::forward<X>(x)), p(p)
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
struct by_adaptor : Projection, F
{
    typedef by_adaptor fit_rewritable_tag;
    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Ts>
    constexpr const Projection& base_projection(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class P, class G, FIT_ENABLE_IF_CONVERTIBLE(P, Projection), FIT_ENABLE_IF_CONVERTIBLE(G, F)>
    constexpr by_adaptor(P&& p, G&& f) 
    : Projection(fit::forward<P>(p)), F(fit::forward<G>(f))
    {}

    FIT_RETURNS_CLASS(by_adaptor);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(const F&, result_of<const Projection&, id_<Ts>>...) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        detail::by_eval(
            FIT_MANGLE_CAST(const Projection&)(FIT_CONST_THIS->base_projection(xs...)),
            FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...)),
            fit::forward<Ts>(xs)...
        )
    );
};

template<class Projection>
struct by_adaptor<Projection, void> : Projection
{
    typedef by_adaptor fit_rewritable1_tag;
    template<class... Ts>
    constexpr const Projection& base_projection(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class P, FIT_ENABLE_IF_CONVERTIBLE(P, Projection)>
    constexpr by_adaptor(P&& p) 
    : Projection(fit::forward<P>(p))
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

}
#endif
