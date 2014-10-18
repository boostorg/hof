/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    lazy.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_LAZY_H
#define FIT_GUARD_FUNCTION_LAZY_H

/// lazy
/// ====
/// 
/// Description
/// -----------
/// 
/// The `lazy` function adaptor returns a function object call wrapper for a
/// function. Calling this wrapper is equivalent to invoking the function. It
/// is a simple form of lambda expressions, but is constexpr friendly.
/// 
/// Ultimately, calling `lazy(f)(x)` is the equivalent to calling
/// `std::bind(f, x)` except the lazy version can be called in a constexpr
/// context, as well. The `lazy` adaptor is compatible with `std::bind`, so
/// most of the time `lazy` and `std::bind` can be used interchangeably.
/// However, the `lazy` adaptor won't accept member function pointers, like
/// `std::bind` will.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr lazy_adaptor<F> lazy(F f);
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
///     auto add = [](auto x, auto y) { return x+y; }
///     auto increment = lazy(add)(_1, 1);
///     assert(increment(5) == 6);
/// 

#include <fit/args.h>
#include <fit/conditional.h>
#include <fit/always.h>
#include <fit/static.h>
#include <fit/invoke.h>
#include <fit/detail/delegate.h>
#include <fit/pack.h>
#include <functional>
#include <type_traits>

namespace fit {

namespace detail {

struct placeholder_transformer
{
    template<class T>
    struct transformer
    {
        template<class... Ts>
        constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
        (args<std::is_placeholder<T>::value>(std::forward<Ts>(xs)...));
    };

    template<class T, typename std::enable_if<(std::is_placeholder<T>::value > 0), int>::type = 0>
    constexpr transformer<T> operator()(const T&) const
    {
        return {};
    }
};

struct bind_transformer
{
    template<class T, typename std::enable_if<std::is_bind_expression<T>::value, int>::type = 0>
    constexpr const T& operator()(const T& x) const
    {
        return x;
    }
};

template<class T>
struct is_reference_wrapper
: std::false_type
{};

template<class T>
struct is_reference_wrapper<std::reference_wrapper<T>>
: std::true_type
{};

struct ref_transformer
{
    template<class T, typename std::enable_if<is_reference_wrapper<T>::value, int>::type = 0>
    constexpr auto operator()(T x) const 
    FIT_RETURNS(always_ref(x.get()));
};

struct id_transformer
{
    template<class T>
    constexpr auto operator()(const T& x) const 
    FIT_RETURNS(always_ref(x));
};

static constexpr conditional_adaptor<placeholder_transformer, bind_transformer, ref_transformer, id_transformer> pick_transformer = {};

template<class T, class Pack>
constexpr auto lazy_transform(T&& x, Pack&& p) FIT_RETURNS
(
    p(fit::detail::pick_transformer(std::forward<T>(x)))
);

template<class F, class Pack>
struct lazy_unpack
{
    const F& f;
    const Pack& p;

    constexpr lazy_unpack(const F& f, const Pack& p) 
    : f(f), p(p)
    {}

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        f(lazy_transform(std::forward<Ts>(xs), p)...)
    );
};

template<class F, class Pack>
constexpr lazy_unpack<F, Pack> make_lazy_unpack(const F& f, const Pack& p)
{
    return lazy_unpack<F, Pack>(f, p);
}

template<class F, class Pack>
struct lazy_invoker : F, Pack
{
    template<class X, class P>
    constexpr lazy_invoker(X&& x, P&& pack) 
    : F(std::forward<X>(x)), Pack(std::forward<P>(pack))
    {}

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Ts>
    constexpr const Pack& get_pack(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(lazy_invoker);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        FIT_MANGLE_CAST(const Pack&)(FIT_CONST_THIS->get_pack(xs...))(
            fit::detail::make_lazy_unpack(FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...)), pack_forward(std::forward<Ts>(xs)...))
        )
    );
};

template<class F, class Pack>
constexpr lazy_invoker<F, Pack> make_lazy_invoker(F f, Pack pack)
{
    return lazy_invoker<F, Pack>(std::move(f), std::move(pack));
}

template<class F>
struct lazy_nullary_invoker : F
{
    FIT_INHERIT_CONSTRUCTOR(lazy_nullary_invoker, F);

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(lazy_nullary_invoker);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        FIT_MANGLE_CAST(const F&)(FIT_CONST_THIS->base_function(xs...))()
    );
};

template<class F>
constexpr lazy_nullary_invoker<F> make_lazy_nullary_invoker(F f)
{
    return lazy_nullary_invoker<F>(std::move(f));
}
}


template<class F>
struct lazy_adaptor : F
{
    FIT_INHERIT_CONSTRUCTOR(lazy_adaptor, F);

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    FIT_RETURNS_CLASS(lazy_adaptor);

    template<class T, class... Ts>
    constexpr auto operator()(T x, Ts... xs) const FIT_RETURNS
    (
        fit::detail::make_lazy_invoker(FIT_RETURNS_C_CAST(F&&)(FIT_CONST_THIS->base_function(x, xs...)), 
            pack(std::move(x), std::move(xs)...))
    );

    // Workaround for gcc 4.7
    template<class Unused=int>
    constexpr detail::lazy_nullary_invoker<F> operator()() const
    {
        return fit::detail::make_lazy_nullary_invoker((F&&)(
            this->base_function(Unused())
        ));
    }

    // TODO: Overloads to use with ref qualifiers

    // template<class... Ts>
    // constexpr auto operator()(Ts... xs) const& FIT_RETURNS
    // (
    //     fit::detail::make_lazy_invoker(this->base_function(xs...), 
    //         pack(std::move(xs)...))
    // );

    // template<class... Ts>
    // constexpr auto operator()(Ts... xs) && FIT_RETURNS
    // (
    //     fit::detail::make_lazy_invoker((F&&)this->base_function(xs...), 
    //         pack(std::move(xs)...))
    // );
    
};

template<class F>
constexpr lazy_adaptor<F> lazy(F f)
{
    return lazy_adaptor<F>(std::move(f));
}

}

namespace std {
    template<class F, class Pack>
    struct is_bind_expression<fit::detail::lazy_invoker<F, Pack>>
    : std::true_type
    {};

    template<class F>
    struct is_bind_expression<fit::detail::lazy_nullary_invoker<F>>
    : std::true_type
    {};
}

#endif