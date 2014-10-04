/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    returns.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_RETURNS_H
#define FIT_GUARD_RETURNS_H

#ifndef FIT_HAS_COMPLETE_DECLTYPE
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8
#define FIT_HAS_COMPLETE_DECLTYPE 0
#else
#define FIT_HAS_COMPLETE_DECLTYPE 1
#endif
#endif

#include <utility>

#if FIT_HAS_COMPLETE_DECLTYPE
#define FIT_RETURNS(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; } static_assert(true, "")
#define FIT_THIS this
#define FIT_CONST_THIS this
#define FIT_RETURNS_CLASS(...) \
void fit_returns_class_check() \
{ \
    static_assert(std::is_same<typename std::remove_cv<__VA_ARGS__>::type, decltype(*this)>::value, "Fit class type doesn't match"); \
}
#else
#include <fit/detail/pp.h>

#define FIT_RETURNS_RETURN(...) return FIT_RETURNS_RETURN_X(FIT_PP_WALL(__VA_ARGS__))
#define FIT_RETURNS_RETURN_X(...) __VA_ARGS__

#define FIT_RETURNS_DECLTYPE(...) decltype(FIT_RETURNS_DECLTYPE_CONTEXT(__VA_ARGS__))

#define FIT_RETURNS_DECLTYPE_CONTEXT(...) FIT_RETURNS_DECLTYPE_CONTEXT_X(FIT_PP_WALL(__VA_ARGS__))
#define FIT_RETURNS_DECLTYPE_CONTEXT_X(...) __VA_ARGS__

#define FIT_RETURNS_THAT(...) FIT_PP_IIF(FIT_PP_IS_PAREN(FIT_RETURNS_DECLTYPE_CONTEXT(())))(\
    (fit::detail::check_this<__VA_ARGS__, decltype(this)>(), this), \
    std::declval<__VA_ARGS__>() \
)

#define FIT_THIS FIT_PP_RAIL(FIT_RETURNS_THAT)(fit_this_type)
#define FIT_CONST_THIS FIT_PP_RAIL(FIT_RETURNS_THAT)(fit_const_this_type)

#define FIT_RETURNS_CLASS(...) typedef __VA_ARGS__* fit_this_type; typedef const __VA_ARGS__* fit_const_this_type

#define FIT_RETURNS(...) -> FIT_RETURNS_DECLTYPE(__VA_ARGS__) { FIT_RETURNS_RETURN(__VA_ARGS__); } static_assert(true, "")

namespace fit { namespace detail {
template<class Assumed, class T>
struct check_this
{
    static_assert(std::is_same<T, Assumed>::value, "Incorret FIT_THIS or FIT_CONST_THIS used.");
};

}}

#endif

#define FIT_AUTO_FORWARD(x) std::forward<decltype(x)>(x)

#endif