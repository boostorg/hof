/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    fuse.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_FUSE_H
#define FIT_GUARD_FUNCTION_FUSE_H

// fuse
// ====
// 
// Description
// -----------
// 
// The `fuse` function adaptor takes a fusion sequence and uses that for the
// arguments to the function.
// 
// Synopsis
// --------
// 
//     template<class F>
//     fuse_adaptor<F> fuse(F f);
// 
// Example
// -------
// 
//     struct sum
//     {
//         template<class T>
//         T sum(T x, T y)
//         {
//             return x+y;
//         }
//     };
// 
//     int r = fuse(sum())(std::make_tuple(3,2));
//     assert(r, 5);
// 

#include <fit/invoke.h>
#include <fit/variadic.h>
#include <fit/always.h>

namespace fit {

template<class F>
struct fuse_adaptor : F
{
    template<class... Ts>
    constexpr fuse_adaptor(Ts && ... x) : F(std::forward<Ts>(x)...)
    {}

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class T>
    constexpr auto operator()(T && x) const
    FIT_RETURNS
    (
        fit::invoke(this->base_function(x), std::forward<T>(x))
    );
};

// Optimizations
template<class F>
struct fuse_adaptor<variadic_adaptor<F> > : F
{
    template<class... Ts>
    constexpr fuse_adaptor(Ts && ... x) : F(std::forward<Ts>(x)...)
    {}
};

template<class F>
struct variadic_adaptor<fuse_adaptor<F> > : F
{
    template<class... Ts>
    constexpr variadic_adaptor(Ts && ... x) : F(std::forward<Ts>(x)...)
    {}
};

template<class F>
constexpr fuse_adaptor<F> fuse(F f)
{
    return fuse_adaptor<F>(f);
}

}

#endif
