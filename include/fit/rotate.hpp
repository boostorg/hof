/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    rotate.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_ROTATE_H
#define FIT_GUARD_ROTATE_H

/// rotate
/// ====
/// 
/// Description
/// -----------
/// 
/// The `rotate` function adaptor moves the first parameter to the last
/// parameter.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     rotate_adaptor<F> rotate(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(rotate(f)(x, xs...) == f(xs..., x));
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
/// 
///     int main() {
///         int r = fit::rotate(fit::_ - fit::_)(2, 5);
///         assert(r == 3);
///     }
/// 

#include <fit/detail/builder.hpp>
#include <fit/detail/builder/unary.hpp>
#include <fit/reveal.hpp>

namespace fit { namespace detail {

struct rotate_adaptor_builder
{
    template<class F>
    struct base
    {
        struct rotate_failure
        {
            template<class Failure>
            struct apply
            {
                template<class T, class... Ts>
                struct of
                : Failure::template of<T, Ts...>
                {};
            };
        };

        struct failure
        : failure_map<rotate_failure, F>
        {};
    };

    struct apply
    {
        template<class F, class T, class... Ts>
        constexpr FIT_SFINAE_RESULT(F&&, id_<Ts>..., id_<T>) 
        operator()(F&& f, T&& x, Ts&&... xs) const FIT_SFINAE_RETURNS
        (
            FIT_FORWARD(F)(f)(FIT_FORWARD(Ts)(xs)..., FIT_FORWARD(T)(x))
        );
    };

};

}

FIT_DECLARE_ADAPTOR(rotate, detail::unary_adaptor_builder<detail::rotate_adaptor_builder>)

} // namespace fit

#endif
