/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    result.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_RESULT_H
#define FIT_GUARD_RESULT_H

/// result
/// ======
/// 
/// Description
/// -----------
/// 
/// The `result` function adaptor sets the return type for the function, which
/// can be useful when dealing with multiple overloads. Since the return type
/// is no longer dependent on the parameters passed to the function, the
/// `result_adaptor` provides a nested `result_type` that is the return type
/// of the function.
/// 
/// Synopsis
/// --------
/// 
///     template<class Result, class F>
///     constexpr result_adaptor<Result, F> result(F f);
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
///     struct id
///     {
///         template<class T>
///         T operator()(T x) const
///         {
///             return x;
///         }
///     };
/// 
///     int main() {
///         auto int_result = fit::result<int>(id());
///         static_assert(std::is_same<decltype(int_result(true)), int>::value, "Not the same type");
///     }
/// 

#include <fit/detail/callable_base.hpp>
#include <fit/is_callable.hpp>
#include <fit/always.hpp>
#include <fit/reveal.hpp>

namespace fit {

template<class Result, class F>
struct result_adaptor : detail::callable_base<F>
{
    FIT_INHERIT_CONSTRUCTOR(result_adaptor, detail::callable_base<F>)

    typedef Result result_type;

    struct failure
    : failure_for<detail::callable_base<F>>
    {};

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const
    {
        return fit::always_ref(*this)(xs...);
    }

    template<class... Ts, class=typename std::enable_if<(fit::is_callable<F, Ts...>::value)>::type>
    constexpr result_type operator()(Ts&&... xs) const
    {
        return this->base_function(xs...)(FIT_FORWARD(Ts)(xs)...);
    };
};

template<class F>
struct result_adaptor<void, F> : detail::callable_base<F>
{
    FIT_INHERIT_CONSTRUCTOR(result_adaptor, detail::callable_base<F>)

    typedef void result_type;

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const
    {
        return fit::always_ref(*this)(xs...);
    }

    template<class... Ts, class=typename std::enable_if<(fit::is_callable<F, Ts...>::value)>::type>
    constexpr typename detail::holder<Ts...>::type operator()(Ts&&... xs) const
    {
        return (typename detail::holder<Ts...>::type)this->base_function(xs...)(FIT_FORWARD(Ts)(xs)...);
    };
};

#if FIT_HAS_VARIABLE_TEMPLATES
namespace result_detail {
template<class Result>
struct result_f
{
    template<class F>
    constexpr result_adaptor<Result, F> operator()(F f) const
    {
        return result_adaptor<Result, F>(std::move(f));
    }
};

}

template<class Result>
static constexpr auto result = result_detail::result_f<Result>{};
#else
template<class Result, class F>
constexpr result_adaptor<Result, F> result(F f)
{
    return result_adaptor<Result, F>(std::move(f));
}
#endif

} // namespace fit

#endif
