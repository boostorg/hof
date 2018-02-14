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
/// another function. It will repeatedly do this until the function can
/// finally be called. By default, `partial` captures all of its variables by
/// value, just like bind. As such all parameters must be `MoveConstructible`
/// when the function is aprtial application. `std::ref` can be used to
/// capture references instead.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr partial_adaptor<F> partial(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(partial(f)(xs...)(ys...) == f(xs..., ys...));
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
///         assert(3 == partial(sum())(1)(2));
///     }
/// 
/// References
/// ----------
/// 
/// * [Partial application](https://en.wikipedia.org/wiki/Partial_application)
/// * [Currying](https://en.wikipedia.org/wiki/Currying)
/// 

#include <fit/conditional.hpp>
#include <fit/static.hpp>
#include <fit/pipable.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/static_const_var.hpp>


namespace fit { 

// TODO: Get rid of sequence parameter
// Forward declare partial_adaptor, since it will be used below
template<class F, class Pack=void >
struct partial_adaptor;

FIT_DECLARE_STATIC_VAR(partial, detail::make<partial_adaptor>);

namespace detail {

template<class Derived, class F, class Pack>
struct partial_adaptor_invoke
{
    template<class... Ts>
    constexpr const F& get_function(Ts&&...) const noexcept
    {
        return static_cast<const F&>(static_cast<const Derived&>(*this));
    }

    template<class... Ts>
    constexpr const Pack& get_pack(Ts&&...) const noexcept
    {
        return static_cast<const Pack&>(static_cast<const Derived&>(*this));
    }

    FIT_RETURNS_CLASS(partial_adaptor_invoke);

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT
    (
        typename result_of<decltype(fit::pack_join), 
            id_<const Pack&>, 
            result_of<decltype(fit::pack_forward), id_<Ts>...> 
        >::type,
        id_<F&&>
    ) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        fit::pack_join
        (
            FIT_MANGLE_CAST(const Pack&)(FIT_CONST_THIS->get_pack(xs...)), 
            fit::pack_forward(FIT_FORWARD(Ts)(xs)...)
        )
        (FIT_RETURNS_C_CAST(F&&)(FIT_CONST_THIS->get_function(xs...)))
    );
};

#ifdef _MSC_VER
#define FIT_PARTIAL_RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); }
#else
#define FIT_PARTIAL_RETURNS FIT_SFINAE_RETURNS
#endif

template<class Derived, class F, class Pack>
struct partial_adaptor_join
{
    template<class... Ts>
    constexpr const F& get_function(Ts&&...) const noexcept
    {
        return static_cast<const F&>(static_cast<const Derived&>(*this));
    }

    template<class... Ts>
    constexpr const Pack& get_pack(Ts&&...) const noexcept
    {
        return static_cast<const Pack&>(static_cast<const Derived&>(*this));
    }

    FIT_RETURNS_CLASS(partial_adaptor_join);

    template<class... Ts, class=typename std::enable_if<
        ((sizeof...(Ts) + Pack::fit_function_param_limit::value) < function_param_limit<F>::value)
    >::type>
    constexpr auto operator()(Ts&&... xs) const 
#ifdef _MSC_VER
    // Workaround ICE on MSVC
    noexcept(FIT_IS_NOTHROW_CONSTRUCTIBLE(F, F&&) && noexcept(fit::pack_join(std::declval<const Pack&>(), fit::pack(FIT_FORWARD(Ts)(xs)...))))
#endif
    FIT_PARTIAL_RETURNS
    (
        fit::partial
        (
            FIT_RETURNS_C_CAST(F&&)(FIT_CONST_THIS->get_function(xs...)), 
            fit::pack_join(FIT_MANGLE_CAST(const Pack&)(FIT_CONST_THIS->get_pack(xs...)), fit::pack(FIT_FORWARD(Ts)(xs)...))
        )
    );
};

template<class Derived, class F>
struct partial_adaptor_pack
{

    constexpr partial_adaptor_pack() noexcept
    {}
    
    template<class... Ts>
    constexpr const F& get_function(Ts&&...) const noexcept
    {
        return static_cast<const F&>(static_cast<const Derived&>(*this));
    }

    FIT_RETURNS_CLASS(partial_adaptor_pack);

    template<class... Ts, class=typename std::enable_if<
        (sizeof...(Ts) < function_param_limit<F>::value)
    >::type>
    constexpr auto operator()(Ts&&... xs) const 
#ifdef _MSC_VER
    // Workaround ICE on MSVC
    noexcept(FIT_IS_NOTHROW_CONSTRUCTIBLE(F, F&&) && noexcept(fit::pack(FIT_FORWARD(Ts)(xs)...)))
#endif
    FIT_PARTIAL_RETURNS
    (
        fit::partial
        (
            FIT_RETURNS_C_CAST(F&&)(FIT_CONST_THIS->get_function(xs...)), 
            fit::pack(FIT_FORWARD(Ts)(xs)...)
        )
    );
};
template<class F, class Pack>
struct partial_adaptor_base 
{
    typedef basic_conditional_adaptor
    <
        partial_adaptor_invoke<partial_adaptor<F, Pack>, F, Pack>,
        partial_adaptor_join<partial_adaptor<F, Pack>, F, Pack> 
    > type;
};

template<class Derived, class F>
struct partial_adaptor_pack_base
{
    typedef basic_conditional_adaptor
    <
        F,
        partial_adaptor_pack<Derived, F> 
    > type;
};

}

template<class F, class Pack>
struct partial_adaptor : detail::partial_adaptor_base<F, Pack>::type, F, Pack
{
    typedef typename detail::partial_adaptor_base<F, Pack>::type base;

    typedef partial_adaptor fit_rewritable1_tag;
    
    template<class... Ts>
    constexpr const F& base_function(Ts&&...) const noexcept
    {
        return *this;
    }

    constexpr const Pack& get_pack() const noexcept
    {
        return *this;
    }

    using base::operator();

    FIT_INHERIT_DEFAULT(partial_adaptor, base, F, Pack);

    template<class X, class S>
    constexpr partial_adaptor(X&& x, S&& seq) 
    FIT_NOEXCEPT(FIT_IS_NOTHROW_CONSTRUCTIBLE(F, X&&) && FIT_IS_NOTHROW_CONSTRUCTIBLE(Pack, S&&))
    : F(FIT_FORWARD(X)(x)), Pack(FIT_FORWARD(S)(seq))
    {}
};

template<class F>
struct partial_adaptor<F, void> : detail::partial_adaptor_pack_base<partial_adaptor<F, void>, detail::callable_base<F>>::type
{
    typedef typename detail::partial_adaptor_pack_base<partial_adaptor<F, void>, detail::callable_base<F>>::type base;

    typedef partial_adaptor fit_rewritable1_tag;
    
    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&...) const noexcept
    {
        return *this;
    }

    using base::operator();

    FIT_INHERIT_CONSTRUCTOR(partial_adaptor, base);

};

// Make partial_adaptor work with pipable_adaptor by removing its pipableness
template<class F>
struct partial_adaptor<pipable_adaptor<F>, void>
: partial_adaptor<F, void>
{
    typedef partial_adaptor<F, void> base;

    typedef partial_adaptor fit_rewritable1_tag;
    FIT_INHERIT_CONSTRUCTOR(partial_adaptor, base);
};

template<class F>
struct partial_adaptor<static_<pipable_adaptor<F>>, void>
: partial_adaptor<F, void>
{
    typedef partial_adaptor<F, void> base;

    typedef partial_adaptor fit_rewritable1_tag;

    FIT_INHERIT_CONSTRUCTOR(partial_adaptor, base);
};
} // namespace fit

#endif
