/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    implicit.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_IMPLICIT_H
#define FIT_GUARD_FUNCTION_IMPLICIT_H

/// implicit
/// ========
/// 
/// Description
/// -----------
/// 
/// The `implicit` adaptor is a static function adaptor that uses the type
/// that the return value can be converted to, in order to determine the type
/// of the template parameter. In essence, it will deduce the type for the
/// template parameter using the type of variable the result is assigned to.
/// Since it is a static function adaptor, the function must be default
/// constructible.
/// 
/// Synopsis
/// --------
/// 
///     template<template <class...> class F>
///     class implicit<F>;
/// 
/// Semantics
/// ---------
/// 
///     assert(T(implicit<F>()(xs...)) == F<T>()(xs...));
/// 
/// Requirements
/// ------------
/// 
/// F must be a template class, that is a:
/// 
/// * [ConstFunctionObject](ConstFunctionObject)
/// * DefaultConstructible
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
///     using namespace fit;
/// 
///     template<class T>
///     struct auto_caster
///     {
///         template<class U>
///         T operator()(U x)
///         {
///             return T(x);
///         }
///     };
/// 
///     static constexpr implicit<auto_caster> auto_cast = {};
/// 
///     struct auto_caster_foo
///     {
///         int i;
///         explicit auto_caster_foo(int i_) : i(i_) {}
/// 
///     };
/// 
///     int main() {
///         float f = 1.5;
///         int i = auto_cast(f);
///         auto_caster_foo x = auto_cast(1);
///         assert(1 == i);
///         assert(1 == x.i);
///     }
/// 

#include <fit/pack.hpp>
#include <fit/detail/result_of.hpp>

namespace fit { namespace detail {

template<class F, class Pack, class X, class=void>
struct is_implicit_callable
: std::false_type
{};

#if FIT_NO_EXPRESSION_SFINAE
template<class F, class Pack, class X>
struct is_implicit_callable<F, Pack, X, typename std::enable_if<
    std::is_convertible<typename result_of<Pack, id_<F>>::type, X>::value
>::type>
: std::true_type
{};
#else
template<class F, class Pack, class X>
struct is_implicit_callable<F, Pack, X, typename std::enable_if<
    std::is_convertible<decltype(std::declval<Pack>()(std::declval<F>())), X>::value
>::type>
: std::true_type
{};
#endif

}


template<template <class...> class F>
struct implicit
{
    template<class Pack>
    struct invoker
    {
        Pack p;

        constexpr invoker(Pack pp) FIT_NOEXCEPT_CONSTRUCTIBLE(Pack, Pack&&)
        : p(fit::move(pp))
        {}

        template<class X, class=typename std::enable_if<detail::is_implicit_callable<F<X>, Pack, X>::value>::type>
        constexpr operator X() const FIT_NOEXCEPT(noexcept(p(F<X>())))
        {
            return p(F<X>());
        }

#if !(defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7)
        invoker (const invoker&) = delete;
        invoker& operator= (const invoker&) = delete;

    private:
        friend struct implicit;
        invoker (invoker&&) = default;
#endif
    };

    struct make_invoker
    {
        template<class Pack>
        constexpr invoker<Pack> operator()(Pack p) const FIT_NOEXCEPT(noexcept(invoker<Pack>(fit::move(p))))
        {
            return invoker<Pack>(fit::move(p));
        }

    };

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const 
    FIT_RETURNS
    (
        FIT_RETURNS_CONSTRUCT(make_invoker)()(fit::pack_basic(FIT_FORWARD(Ts)(xs)...))
    );
};

} // namespace fit

#endif
