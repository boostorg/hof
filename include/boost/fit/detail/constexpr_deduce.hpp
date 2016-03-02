/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    constexpr_deduce.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FUNCTION_CONSTEXPR_DEDUCE_H
#define BOOST_FIT_GUARD_FUNCTION_CONSTEXPR_DEDUCE_H


#ifndef BOOST_FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR
#if defined(_MSC_VER)
#define BOOST_FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR 1
#else
#define BOOST_FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR 0
#endif
#endif

#define BOOST_FIT_CONST_FOLD(x) (__builtin_constant_p(x) ? (x) : (x))

namespace boost { namespace fit {

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
#if BOOST_FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR
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
        static_assert(BOOST_FIT_IS_EMPTY(F), "Function or lambda expression must be empty");
        return BOOST_FIT_CONST_FOLD(reinterpret_cast<const F&>(static_const_var<T>()));
    }
#endif
};

}}} // namespace boost::fit

#define BOOST_FIT_DETAIL_CONSTEXPR_DEDUCE true ? boost::fit::detail::constexpr_deduce() :
#define BOOST_FIT_DETAIL_CONSTEXPR_DEDUCE_UNIQUE(T) true ? boost::fit::detail::constexpr_deduce_unique<T>() :

#ifdef _MSC_VER
#define BOOST_FIT_DETAIL_MSVC_CONSTEXPR_DEDUCE BOOST_FIT_DETAIL_CONSTEXPR_DEDUCE
#else
#define BOOST_FIT_DETAIL_MSVC_CONSTEXPR_DEDUCE
#endif

#endif
