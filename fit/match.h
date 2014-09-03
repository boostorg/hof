/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    match.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_OVERLOAD_H
#define FIT_GUARD_FUNCTION_OVERLOAD_H

/// match
/// =====
/// 
/// Description
/// -----------
/// 
/// The `match` function adaptor combines several functions together and
/// resolves which one should be called by using C++ overload resolution. This
/// is different than than the `conditional` adaptor which resolves them based
/// on order.
/// 
/// Synopsis
/// --------
/// 
///     template<class... Fs>
///     constexpr match_adaptor<Fs...> match(Fs...fs);
/// 
/// Requirements
/// ------------
/// 
/// Fs must be:
/// 
///     FunctionObject
///     MoveConstructible
/// 
/// Example
/// -------
/// 
///     struct int_class
///     {
///         int operator()(int) const
///         {
///             return 1;
///         }
///     };
/// 
///     struct foo
///     {};
/// 
///     struct foo_class
///     {
///         foo operator()(foo) const
///         {
///             return foo();
///         }
///     };
/// 
///     typedef match_adaptor<int_class, foo_class> fun;
/// 
///     static_assert(std::is_same<int, decltype(fun()(1))>::value, "Failed match");
///     static_assert(std::is_same<foo, decltype(fun()(foo()))>::value, "Failed match");
/// 

#include <fit/detail/delegate.h>

namespace fit {

template<class...Fs> struct match_adaptor;
 
template<class F, class...Fs>
struct match_adaptor<F, Fs...> : F, match_adaptor<Fs...>
{
    typedef match_adaptor<Fs...> base;

    constexpr match_adaptor()
    {}

    template<class X, class... Xs, FIT_ENABLE_IF_CONVERTIBLE(X, F), FIT_ENABLE_IF_CONSTRUCTIBLE(base, Xs...)>
    constexpr match_adaptor(X&& f1, Xs&& ... fs) 
    : F(std::forward<X>(f1)), base(std::forward<Xs>(fs)...)
    {}

    using F::operator();
    using base::operator();
};

template<class F>
struct match_adaptor<F> : F
{
    typedef F base;
    using F::operator();

    FIT_INHERIT_CONSTRUCTOR(match_adaptor, F);
};

template<class...Fs>
constexpr match_adaptor<Fs...> match(Fs...fs)
{ 
    return match_adaptor<Fs...>(std::move(fs)...); 
}

}

#endif
