/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    flow.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_FLOW_H
#define FIT_GUARD_FUNCTION_FLOW_H

/// flow
/// ====
/// 
/// Description
/// -----------
/// 
/// The `flow` function adaptor provides function composition. It is useful as
/// an alternative to using the pipe operator `|` when chaining functions. It is
/// similiar to [`compose`](compose.md) except the evaluation order is
/// reversed. So, `flow(f, g)(0)` is equivalent to `g(f(0))`.
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class... Fs>
///     constexpr flow_adaptor<Fs...> flow(Fs... fs);
/// 
/// Semantics
/// ---------
/// 
///     assert(flow(f, g)(xs...) == g(f(xs...)));
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
///         int r = flow(increment(), decrement(), increment())(3);
///         assert(r == 4);
///     }
/// 
/// References
/// ----------
/// 
/// * [Function composition](https://en.wikipedia.org/wiki/Function_composition)
/// 

#include <fit/detail/builder.hpp>
#include <fit/detail/builder/fold.hpp>
#include <fit/reveal.hpp>

namespace fit { namespace detail {

struct flow_adaptor_base
{
    template<class F, class G>
    struct base
    {
        struct flow_failure
        {
            template<class Failure>
            struct apply
            {
                template<class... Ts>
                struct of
                : Failure::template of<decltype(std::declval<F>()(std::declval<Ts>()...))>
                {};
            };
        };

        struct failure
        : with_failures<failure_map<flow_failure, G>, failure_for<F>>
        {};
    };

    struct apply
    {
        template<class F1, class F2, class... Ts>
        constexpr FIT_SFINAE_RESULT(const F1&, result_of<const F2&, id_<Ts>...>)
        operator()(F1&& f1, F2&& f2, Ts && ... xs) const
        FIT_SFINAE_RETURNS
        (
            FIT_FORWARD(F2)(f2)(FIT_FORWARD(F1)(f1)(FIT_FORWARD(Ts)(xs)...))
        );
    };
};
}

FIT_DECLARE_ADAPTOR(flow, detail::fold_adaptor_builder<detail::flow_adaptor_base>)

} // namespace fit

#endif
