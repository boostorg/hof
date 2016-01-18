/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    constexpr_deduce.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_CONSTEXPR_DEDUCE_H
#define FIT_GUARD_FUNCTION_CONSTEXPR_DEDUCE_H


#ifndef FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR
#if defined(_MSC_VER)
#define FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR 1
#else
#define FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR 0
#endif
#endif

#define FIT_CONST_FOLD(x) (__builtin_constant_p(x) ? (x) : (x))

namespace fit {

namespace detail {

struct constexpr_deduce
{
    constexpr constexpr_deduce()
    {}
    template<class F>
    constexpr operator F() const
    {
        return F();
    }
};

template<class T>
struct constexpr_deduce_unique
{
    constexpr constexpr_deduce_unique()
    {}
#if FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR
    template<class F>
    constexpr operator F() const
    {
        // static_assert(std::is_default_constructible<F>::value, "Function not default constructible");
        return F();
    }
#else
    template<class F>
    constexpr operator const F&() const
    {
        static_assert(std::is_empty<F>::value, "Function or lambda expression must be empty");
        return FIT_CONST_FOLD(reinterpret_cast<const F&>(static_const_var<T>()));
    }
#endif
};

}} // namespace fit

#define FIT_DETAIL_CONSTEXPR_DEDUCE true ? fit::detail::constexpr_deduce() :
#define FIT_DETAIL_CONSTEXPR_DEDUCE_UNIQUE(T) true ? fit::detail::constexpr_deduce_unique<T>() :

#ifdef _MSC_VER
#define FIT_DETAIL_MSVC_CONSTEXPR_DEDUCE FIT_DETAIL_CONSTEXPR_DEDUCE
#else
#define FIT_DETAIL_MSVC_CONSTEXPR_DEDUCE
#endif

#endif
