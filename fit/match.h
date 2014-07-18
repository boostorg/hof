/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    match.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZEN_GUARD_FUNCTION_OVERLOAD_H
#define ZEN_GUARD_FUNCTION_OVERLOAD_H

// match
// =====
// 
// Description
// -----------
// 
// The `match` function adaptor combines several functions together and
// resolves which one should be called by using C++ overload resolution. This is
// different than than the `conditional` adaptor which resolves them based on
// order.
// 
// Synopsis
// --------
// 
//     template<class F1, class F2, ...>
//     match_adaptor<F1, F2, ...> match(F1 f1, F2 f2, ...);
// 
// Example
// -------
// 
//     struct int_class
//     {
//         int operator()(int) const
//         {
//             return 1;
//         }
//     };
// 
//     struct foo
//     {};
// 
//     struct foo_class
//     {
//         foo operator()(foo) const
//         {
//             return foo();
//         }
//     };
// 
//     typedef match_adaptor<int_class, foo_class> fun;
// 
//     static_assert(std::is_same<int, decltype(fun()(1))>::value, "Failed match");
//     static_assert(std::is_same<foo, decltype(fun()(foo()))>::value, "Failed match");
// 


namespace fit {

namespace detail {
template<class...Fs> struct match_adaptor_base;
 
template<class F, class...Fs>
struct match_adaptor_base<F, Fs...> : F, match_adaptor_base<Fs...>
{
    typedef match_adaptor_base<Fs...> base;

    match_adaptor_base()
    {}

    template<class T, class... Ts>
    match_adaptor_base(T head, Ts... tail)
    : F(head), base(tail...)
    {}

    using F::operator();
    using base::operator();
};

template<class F>
struct match_adaptor_base<F> : F
{
    typedef F base;
    using F::operator();

    match_adaptor_base()
    {}

    template<class T>
    match_adaptor_base(T f) : F(f)
    {}
};
}

template<class...Fs> 
struct match_adaptor : detail::match_adaptor_base<Fs...>
{
    typedef detail::match_adaptor_base<Fs...> base;
    match_adaptor()
    {}

    template<class... T>
    match_adaptor(T... x) : base(x...)
    {}
};

template<class...Fs>
typename match_adaptor<Fs...>::type match(Fs...x)
{ 
    return match_adaptor<Fs...>(x...); 
}

}

#ifdef ZEN_TEST
#include <fit/test.h>

namespace fit { namespace match_test {
struct int_class
{
    int operator()(int) const
    {
        return 1;
    }
};

struct foo
{};

struct foo_class
{
    foo operator()(foo) const
    {
        return foo();
    }
};

fit::static_<fit::match_adaptor<int_class, foo_class> > fun = {};

static_assert(boost::is_same<int, decltype(fun(1))>::value, "Failed match");
static_assert(boost::is_same<foo, decltype(fun(foo()))>::value, "Failed match");
}}

#endif

#endif
