/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    constexpr_deduce.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_CONSTEXPR_DEDUCE_H
#define FIT_GUARD_FUNCTION_CONSTEXPR_DEDUCE_H

#include <fit/config.hpp>

#define FIT_CONST_FOLD(x) (__builtin_constant_p(x) ? (x) : (x))

#ifndef FIT_HAS_CONST_FOLD
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define FIT_HAS_CONST_FOLD 0
#elif defined(__clang__) || defined (__GNUC__)
#define FIT_HAS_CONST_FOLD 1
#else
#define FIT_HAS_CONST_FOLD 0
#endif
#endif


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
#if FIT_HAS_CONST_FOLD
    template<class F>
    constexpr operator const F&() const
    {
        static_assert(FIT_IS_EMPTY(F), "Function or lambda expression must be empty");
        return FIT_CONST_FOLD(reinterpret_cast<const F&>(static_const_var<T>()));
    }
#else
    template<class F>
    constexpr operator F() const
    {
        // static_assert(std::is_default_constructible<F>::value, "Function not default constructible");
        return F();
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
