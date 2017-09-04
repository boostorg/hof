/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    compose.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_COMPOSE_H
#define FIT_GUARD_FUNCTION_COMPOSE_H

/// compose
/// =======
/// 
/// Description
/// -----------
/// 
/// The `compose` function adaptor provides function composition. It produces
/// a function object that composes a set of functions, ie the output of one
/// function becomes the input of the second function. So, `compose(f, g)(0)`
/// is equivalent to `f(g(0))`.
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class... Fs>
///     constexpr compose_adaptor<Fs...> compose(Fs... fs);
/// 
/// Semantics
/// ---------
/// 
///     assert(compose(f, g)(xs...) == f(g(xs...)));
/// 
/// Requirements
/// ------------
/// 
/// Fs must be:
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
///     struct increment
///     {
///         template<class T>
///         T operator()(T x) const
///         {
///             return x + 1;
///         }
///     };
/// 
///     struct decrement
///     {
///         template<class T>
///         T operator()(T x) const
///         {
///             return x - 1;
///         }
///     };
/// 
///     int main() {
///         int r = compose(increment(), decrement(), increment())(3);
///         assert(r == 4);
///     }
/// 
/// References
/// ----------
/// 
/// * [Function composition](https://en.wikipedia.org/wiki/Function_composition)
/// 
/// 

#include <fit/detail/builder.hpp>
#include <fit/detail/builder/fold.hpp>
#include <fit/reveal.hpp>

namespace fit { namespace detail {

struct compose_adaptor_base
{
    template<class F, class G>
    struct base
    {
        struct compose_failure
        {
            template<class Failure>
            struct apply
            {
                template<class... Ts>
                struct of
                : Failure::template of<decltype(std::declval<G>()(std::declval<Ts>()...))>
                {};
            };
        };

        struct failure
        : with_failures<failure_map<compose_failure, F>, failure_for<G>>
        {};
    };

    struct apply
    {
        template<class F1, class F2, class... Ts>
        constexpr FIT_SFINAE_RESULT(const F1&, result_of<const F2&, id_<Ts>...>)
        operator()(F1&& f1, F2&& f2, Ts && ... xs) const
        FIT_SFINAE_RETURNS
        (
            FIT_FORWARD(F1)(f1)(FIT_FORWARD(F2)(f2)(FIT_FORWARD(Ts)(xs)...))
        );
    };
};
}

FIT_DECLARE_ADAPTOR(compose, detail::fold_adaptor_builder<detail::compose_adaptor_base>)

} // namespace fit

#endif
