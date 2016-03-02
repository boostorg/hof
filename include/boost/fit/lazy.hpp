/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    lazy.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FUNCTION_LAZY_H
#define BOOST_FIT_GUARD_FUNCTION_LAZY_H

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
/// * [ConstCallable](concepts.md#constcallable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     auto add = [](auto x, auto y) { return x+y; };
///     auto increment = lazy(add)(_1, 1);
///     assert(increment(5) == 6);
/// 

#include <boost/fit/arg.hpp>
#include <boost/fit/conditional.hpp>
#include <boost/fit/always.hpp>
#include <boost/fit/static.hpp>
#include <boost/fit/detail/delegate.hpp>
#include <boost/fit/detail/compressed_pair.hpp>
#include <boost/fit/pack.hpp>
#include <boost/fit/detail/make.hpp>
#include <boost/fit/detail/static_const_var.hpp>
#include <functional>
#include <type_traits>

namespace boost { namespace fit {

namespace detail {

struct placeholder_transformer
{
    template<class T>
    struct transformer
    {
        template<class... Ts>
        constexpr auto operator()(Ts&&... xs) const BOOST_FIT_RETURNS
        (detail::get_args<std::is_placeholder<T>::value>(BOOST_FIT_FORWARD(Ts)(xs)...));
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
    BOOST_FIT_SFINAE_RETURNS(always_ref(x.get()));
};

struct id_transformer
{
    template<class T>
    constexpr auto operator()(const T& x) const 
    BOOST_FIT_SFINAE_RETURNS(always_ref(x));
};

BOOST_FIT_DECLARE_STATIC_VAR(pick_transformer, conditional_adaptor<placeholder_transformer, bind_transformer, ref_transformer, id_transformer>);

template<class T, class Pack>
constexpr auto lazy_transform(T&& x, Pack&& p) BOOST_FIT_RETURNS
(
    p(boost::fit::detail::pick_transformer(BOOST_FIT_FORWARD(T)(x)))
);

template<class F, class Pack>
struct lazy_unpack
{
    const F& f;
    const Pack& p;

    constexpr lazy_unpack(const F& fp, const Pack& pp) 
    : f(fp), p(pp)
    {}

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const BOOST_FIT_RETURNS
    (
        f(lazy_transform(BOOST_FIT_FORWARD(Ts)(xs), p)...)
    );
};

template<class F, class Pack>
constexpr lazy_unpack<F, Pack> make_lazy_unpack(const F& f, const Pack& p)
{
    return lazy_unpack<F, Pack>(f, p);
}

template<class F, class Pack>
struct lazy_invoker 
: detail::compressed_pair<F, Pack>
{
    typedef detail::compressed_pair<F, Pack> base_type;
    typedef lazy_invoker fit_rewritable1_tag;

#ifdef _MSC_VER
    BOOST_FIT_INHERIT_CONSTRUCTOR(lazy_invoker, base_type)
#else
    BOOST_FIT_INHERIT_DEFAULT_EMPTY(lazy_invoker, base_type)

    template<class X, class Y, 
        BOOST_FIT_ENABLE_IF_CONSTRUCTIBLE(base_type, X&&, Y&&)
    >
    constexpr lazy_invoker(X&& x, Y&& y) 
    : base_type(BOOST_FIT_FORWARD(X)(x), BOOST_FIT_FORWARD(Y)(y))
    {}
#endif

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return this->first(xs...);
    }

    template<class... Ts>
    constexpr const Pack& get_pack(Ts&&... xs) const
    {
        return this->second(xs...);
    }

    BOOST_FIT_RETURNS_CLASS(lazy_invoker);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const BOOST_FIT_RETURNS
    (
        BOOST_FIT_MANGLE_CAST(const Pack&)(BOOST_FIT_CONST_THIS->get_pack(xs...))(
            boost::fit::detail::make_lazy_unpack(
                BOOST_FIT_MANGLE_CAST(const F&)(BOOST_FIT_CONST_THIS->base_function(xs...)), 
                pack_forward(BOOST_FIT_FORWARD(Ts)(xs)...)
            )
        )
    );
};

template<class F, class Pack>
constexpr lazy_invoker<F, Pack> make_lazy_invoker(F f, Pack pack)
{
    return lazy_invoker<F, Pack>(static_cast<F&&>(f), static_cast<Pack&&>(pack));
}

template<class F>
struct lazy_nullary_invoker : F
{
    BOOST_FIT_INHERIT_CONSTRUCTOR(lazy_nullary_invoker, F);

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    BOOST_FIT_RETURNS_CLASS(lazy_nullary_invoker);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const BOOST_FIT_RETURNS
    (
        BOOST_FIT_MANGLE_CAST(const F&)(BOOST_FIT_CONST_THIS->base_function(xs...))()
    );
};

template<class F>
constexpr lazy_nullary_invoker<F> make_lazy_nullary_invoker(F f)
{
    return lazy_nullary_invoker<F>(static_cast<F&&>(f));
}
}


template<class F>
struct lazy_adaptor : detail::callable_base<F>
{
    BOOST_FIT_INHERIT_CONSTRUCTOR(lazy_adaptor, detail::callable_base<F>);

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    BOOST_FIT_RETURNS_CLASS(lazy_adaptor);

    template<class T, class... Ts>
    constexpr auto operator()(T x, Ts... xs) const BOOST_FIT_RETURNS
    (
        boost::fit::detail::make_lazy_invoker(BOOST_FIT_RETURNS_C_CAST(detail::callable_base<F>&&)(BOOST_FIT_CONST_THIS->base_function(x, xs...)), 
            pack(BOOST_FIT_RETURNS_STATIC_CAST(T&&)(x), BOOST_FIT_RETURNS_STATIC_CAST(Ts&&)(xs)...))
    );

    // Workaround for gcc 4.7
    template<class Unused=int>
    constexpr detail::lazy_nullary_invoker<F> operator()() const
    {
        return boost::fit::detail::make_lazy_nullary_invoker((detail::callable_base<F>&&)(
            this->base_function(Unused())
        ));
    }

    // TODO: Overloads to use with ref qualifiers

    // template<class... Ts>
    // constexpr auto operator()(Ts... xs) const& BOOST_FIT_RETURNS
    // (
    //     boost::fit::detail::make_lazy_invoker(this->base_function(xs...), 
    //         pack(boost::fit::move(xs)...))
    // );

    // template<class... Ts>
    // constexpr auto operator()(Ts... xs) && BOOST_FIT_RETURNS
    // (
    //     boost::fit::detail::make_lazy_invoker((F&&)this->base_function(xs...), 
    //         pack(boost::fit::move(xs)...))
    // );
    
};

BOOST_FIT_DECLARE_STATIC_VAR(lazy, detail::make<lazy_adaptor>);

}} // namespace boost::fit

namespace std {
    template<class F, class Pack>
    struct is_bind_expression<boost::fit::detail::lazy_invoker<F, Pack>>
    : std::true_type
    {};

    template<class F>
    struct is_bind_expression<boost::fit::detail::lazy_nullary_invoker<F>>
    : std::true_type
    {};
}

#endif
