/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    indirect.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_INDIRECT_H
#define FIT_GUARD_FUNCTION_INDIRECT_H

/// indirect
/// ========
/// 
/// Description
/// -----------
/// 
/// The `indirect` function adaptor dereferences the object before calling it.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr indirect_adaptor<F> indirect(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(indirect(f)(xs...) == (*f)(xs...));
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * MoveConstructible
/// * Dereferenceable
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
///     #include <memory>
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
///         int r = indirect(std::make_unique<sum>())(3,2);
///         assert(r == 5);
///     }
/// 

#include <fit/detail/builder.hpp>
#include <fit/detail/builder/fold.hpp>
#include <fit/reveal.hpp>

namespace fit { namespace detail {

template<class F>
struct pointer_base
{
    F f;
    constexpr pointer_base() noexcept
    {}

    constexpr pointer_base(F x) noexcept
    : f(x)
    {}

    constexpr auto operator*() const FIT_RETURNS(*f);
};



struct indirect_adaptor_base
{
    template<class F>
    struct base
    {
        struct failure
        : failure_for<decltype(*std::declval<F>())>
        {};
    };

    struct apply
    {
        template<class F, class... Ts>
        constexpr FIT_SFINAE_RESULT(decltype(*std::declval<F>()), id_<Ts>...) 
        operator()(const F& f, Ts&&... xs) const FIT_SFINAE_RETURNS
        (
            (*f)(FIT_FORWARD(Ts)(xs)...)
        );
    };
};
}

template<class F>
FIT_DECLARE_ADAPTOR_USING(indirect, FIT_BUILDER_JOIN_BASE(
    detail::unary_adaptor_builder<detail::indirect_adaptor_base>
)(typename std::conditional<std::is_pointer<F>::value, detail::pointer_base<F>, F>::type))

} // namespace fit

#endif
