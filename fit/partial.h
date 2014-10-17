/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    partial.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_PARTIAL_H
#define FIT_GUARD_FUNCTION_PARTIAL_H

/// partial
/// ========
/// 
/// Description
/// -----------
/// 
/// The `partial` function adaptor allows partial application of the function.
/// If the function can not be called with all the parameters, it will return
/// another function. It will continually do this until the function can
/// finally be called. By default, `partial` captures all of it variables by
/// value, just like bind. `std::ref` can be used to capture references
/// instead.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr partial_adaptor<F> partial(F f);
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
///     assert(3 == partial(sum())(1)(2));
/// 

#include <fit/conditional.h>
#include <fit/static.h>
#include <fit/invoke.h>
#include <fit/pipable.h>
#include <fit/fuse.h>



namespace fit { 

// TODO: Get rid of sequence parameter
// Forward declare partial_adaptor, since it will be used below
template<class F, class Pack=void >
struct partial_adaptor;

template<class F>
constexpr partial_adaptor<F> partial(F f)
{
    return partial_adaptor<F>(std::move(f));
}

template<class F, class Pack>
constexpr partial_adaptor<F, Pack> partial(F f, Pack pack)
{
    return partial_adaptor<F, Pack>(std::move(f), std::move(pack));
}

namespace detail {

template<class Derived, class F, class Pack>
struct partial_adaptor_invoke
{
    template<class... Ts>
    constexpr const F& get_function(Ts&&...) const
    {
        return static_cast<const F&>(static_cast<const Derived&>(*this));
    }

    template<class... Ts>
    constexpr const Pack& get_pack(Ts&&...) const
    {
        return static_cast<const Pack&>(static_cast<const Derived&>(*this));
    }

    FIT_RETURNS_CLASS(partial_adaptor_invoke);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        fit::pack_join
        (
            FIT_MANGLE_CAST(const Pack&)(FIT_CONST_THIS->get_pack(xs...)), 
            fit::pack_forward(std::forward<Ts>(xs)...)
        )
        (FIT_RETURNS_C_CAST(F&&)(FIT_CONST_THIS->get_function(xs...)))
    );
};


template<class Derived, class F, class Pack>
struct partial_adaptor_join
{
    template<class... Ts>
    constexpr const F& get_function(Ts&&...) const
    {
        return static_cast<const F&>(static_cast<const Derived&>(*this));
    }

    template<class... Ts>
    constexpr const Pack& get_pack(Ts&&...) const
    {
        return static_cast<const Pack&>(static_cast<const Derived&>(*this));
    }

    FIT_RETURNS_CLASS(partial_adaptor_join);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        partial
        (
            FIT_RETURNS_C_CAST(F&&)(FIT_CONST_THIS->get_function(xs...)), 
            fit::pack_join(FIT_MANGLE_CAST(const Pack&)(FIT_CONST_THIS->get_pack(xs...)), fit::pack_decay(std::forward<Ts>(xs)...))
        )
    );
};
template<class Derived, class F>
struct partial_adaptor_pack
{
    template<class... Ts>
    constexpr const F& get_function(Ts&&...) const
    {
        return static_cast<const F&>(static_cast<const Derived&>(*this));
    }

    FIT_RETURNS_CLASS(partial_adaptor_pack);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        partial
        (
            FIT_RETURNS_C_CAST(F&&)(FIT_CONST_THIS->get_function(xs...)), 
            fit::pack_decay(std::forward<Ts>(xs)...)
        )
    );
};
template<class F, class Pack>
struct partial_adaptor_base 
{
    typedef conditional_adaptor
    <
        partial_adaptor_invoke<partial_adaptor<F, Pack>, F, Pack>,
        partial_adaptor_join<partial_adaptor<F, Pack>, F, Pack> 
    > type;
};

template<class F>
struct partial_adaptor_base<F, void>
{
    typedef conditional_adaptor
    <
        F,
        partial_adaptor_pack<partial_adaptor<F, void>, F> 
    > type;
};

}

template<class F, class Pack>
struct partial_adaptor : detail::partial_adaptor_base<F, Pack>::type, F, Pack
{
    typedef typename detail::partial_adaptor_base<F, Pack>::type base;
    
    template<class... Ts>
    constexpr const F& base_function(Ts&&...) const
    {
        return *this;
    }

    constexpr const Pack& get_pack() const
    {
        return *this;
    }

    using base::operator();

    constexpr partial_adaptor()
    {}

    template<class X, class S>
    constexpr partial_adaptor(X&& x, S&& seq) : F(std::forward<X>(x)), Pack(std::forward<S>(seq))
    {}
};

template<class F>
struct partial_adaptor<F, void> : detail::partial_adaptor_base<F, void>::type
{
    typedef typename detail::partial_adaptor_base<F, void>::type base;
    
    template<class... Ts>
    constexpr const F& base_function(Ts&&...) const
    {
        return *this;
    }

    using base::operator();

    constexpr partial_adaptor()
    {}

    template<class X, FIT_ENABLE_IF_CONVERTIBLE(X, base)>
    constexpr partial_adaptor(X&& x) : base(std::forward<X>(x))
    {}


};
// Make partial_adaptor work with pipable_adaptor by removing its pipableness
template<class F>
struct partial_adaptor<pipable_adaptor<F>, void>
: partial_adaptor<F, void>
{
    typedef partial_adaptor<F, void> base;
    constexpr partial_adaptor()
    {}

    template<class X, FIT_ENABLE_IF_CONVERTIBLE(X, base)>
    constexpr partial_adaptor(X&& x) : base(std::forward<X>(x))
    {}
};

template<class F>
struct partial_adaptor<static_<pipable_adaptor<F>>, void>
: partial_adaptor<F, void>
{
    typedef partial_adaptor<F, void> base;
    partial_adaptor()
    {}

    template<class X, FIT_ENABLE_IF_CONVERTIBLE(X, base)>
    partial_adaptor(X&& x) : base(std::forward<X>(x))
    {}
};
}

#endif
