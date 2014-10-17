/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    returns.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_RETURNS_H
#define FIT_GUARD_RETURNS_H

/// returns
/// =======
/// 
/// Description
/// -----------
/// 
/// The `FIT_RETURNS` macro helps deduce the return type of an expression
/// using the trailing return type. Instead of repeating the expression for
/// the return type and using the expression in the function body, this macro
/// helps reduce the code duplication from that.
/// 
/// Note: A semicolon must be placed at the end of the `FIT_RETURNS`
/// expression.
/// 
/// Note: The expression used to deduce the return the type will also
/// constrain the template function as well, which is different behaviour than
/// using C++14's return type deduction.
/// 
/// Synopsis
/// --------
/// 
///     #define FIT_RETURNS(...) 
/// 
/// 
/// Example
/// -------
/// 
///     template<class T, class U>
///     auto sum(T x, U y) FIT_RETURNS(x+y);
/// 
///     assert(3 == sum(1, 2));
/// 
/// 
/// Incomplete this
/// ===============
/// 
/// Description
/// -----------
/// 
/// On older compilers the `this` variable cannot be used inside the
/// `FIT_RETURNS` macro because it is considered an incomplete type. So the
/// following macros are provided to help workaround the issue.
/// 
/// 
/// Synopsis
/// --------
/// 
///     // Declares the type of the `this` variable
///     #define FIT_RETURNS_CLASS(...) 
///     // Used to refer to the `this` variable in the FIT_RETURNS macro
///     #define FIT_THIS
///     // Used to refer to the const `this` variable in the FIT_RETURNS macro
///     #define FIT_CONST_THIS
/// 
/// 
/// Example
/// -------
/// 
///     struct add_1
///     {
///         int a;
///         add_1() : a(1) {}
///         
///         FIT_RETURNS_CLASS(add_1)
///         
///         template<class T>
///         auto operator()(T x) const 
///         FIT_RETURNS(x+FIT_CONST_THIS->a);
///     };
/// 
///     assert(3 == add_1()(2));
/// 
/// 
/// Mangling overloads
/// ==================
/// 
/// Description
/// -----------
/// 
/// On older compilers some operations done in the exressions cannot be
/// properly mangled. These macros help provide workarounds for these
/// operations used on these older compilers.
/// 
/// 
/// Synopsis
/// --------
/// 
///     // Explicitly defines the type for name mangling
///     #define FIT_MANGLE_CAST(...) 
///     // C cast for name mangling
///     #define FIT_RETURNS_C_CAST(...) 
///     // Reinterpret cast for name mangling
///     #define FIT_RETURNS_REINTERPRET_CAST(...) 
///     // Static cast for name mangling
///     #define FIT_RETURNS_STATIC_CAST(...) 
///     // Construction for name mangling
///     #define FIT_RETURNS_CONSTRUCT(...) 
/// 


#ifndef FIT_HAS_MANGLE_OVERLOAD
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define FIT_HAS_MANGLE_OVERLOAD 0
#else
#define FIT_HAS_MANGLE_OVERLOAD 1
#endif
#endif

#ifndef FIT_HAS_COMPLETE_DECLTYPE
#if !FIT_HAS_MANGLE_OVERLOAD || (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8)
#define FIT_HAS_COMPLETE_DECLTYPE 0
#else
#define FIT_HAS_COMPLETE_DECLTYPE 1
#endif
#endif

#include <utility>

#define FIT_EAT(...)
#define FIT_REM(...) __VA_ARGS__

#if FIT_HAS_COMPLETE_DECLTYPE && FIT_HAS_MANGLE_OVERLOAD
#define FIT_RETURNS(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; } static_assert(true, "")
#define FIT_THIS this
#define FIT_CONST_THIS this
#define FIT_RETURNS_CLASS(...) \
void fit_returns_class_check() \
{ \
    static_assert(std::is_same<typename std::remove_cv<__VA_ARGS__>::type, decltype(*this)>::value, "Fit class type doesn't match"); \
}

#define FIT_MANGLE_CAST(...) FIT_REM

#define FIT_RETURNS_C_CAST(...) (__VA_ARGS__) FIT_REM
#define FIT_RETURNS_REINTERPRET_CAST(...) reinterpret_cast<__VA_ARGS__>
#define FIT_RETURNS_STATIC_CAST(...) static_cast<__VA_ARGS__>
#define FIT_RETURNS_CONSTRUCT(...) __VA_ARGS__
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


#if FIT_HAS_MANGLE_OVERLOAD

#define FIT_MANGLE_CAST(...) FIT_REM

#define FIT_RETURNS_C_CAST(...) (__VA_ARGS__) FIT_REM
#define FIT_RETURNS_REINTERPRET_CAST(...) reinterpret_cast<__VA_ARGS__>
#define FIT_RETURNS_STATIC_CAST(...) static_cast<__VA_ARGS__>
#define FIT_RETURNS_CONSTRUCT(...) __VA_ARGS__

#else

#define FIT_RETURNS_DERAIL_MANGLE_CAST(...) FIT_PP_IIF(FIT_PP_IS_PAREN(FIT_RETURNS_DECLTYPE_CONTEXT(())))(\
    FIT_REM, \
    std::declval<__VA_ARGS__>() FIT_EAT \
)
#define FIT_MANGLE_CAST FIT_PP_RAIL(FIT_RETURNS_DERAIL_MANGLE_CAST)


#define FIT_RETURNS_DERAIL_C_CAST(...) FIT_PP_IIF(FIT_PP_IS_PAREN(FIT_RETURNS_DECLTYPE_CONTEXT(())))(\
    (__VA_ARGS__) FIT_REM, \
    std::declval<__VA_ARGS__>() FIT_EAT \
)
#define FIT_RETURNS_C_CAST FIT_PP_RAIL(FIT_RETURNS_DERAIL_C_CAST)


#define FIT_RETURNS_DERAIL_REINTERPRET_CAST(...) FIT_PP_IIF(FIT_PP_IS_PAREN(FIT_RETURNS_DECLTYPE_CONTEXT(())))(\
    reinterpret_cast<__VA_ARGS__>, \
    std::declval<__VA_ARGS__>() FIT_EAT \
)
#define FIT_RETURNS_REINTERPRET_CAST FIT_PP_RAIL(FIT_RETURNS_DERAIL_REINTERPRET_CAST)

#define FIT_RETURNS_DERAIL_STATIC_CAST(...) FIT_PP_IIF(FIT_PP_IS_PAREN(FIT_RETURNS_DECLTYPE_CONTEXT(())))(\
    static_cast<__VA_ARGS__>, \
    std::declval<__VA_ARGS__>() FIT_EAT \
)
#define FIT_RETURNS_STATIC_CAST FIT_PP_RAIL(FIT_RETURNS_DERAIL_STATIC_CAST)

#define FIT_RETURNS_DERAIL_CONSTRUCT(...) FIT_PP_IIF(FIT_PP_IS_PAREN(FIT_RETURNS_DECLTYPE_CONTEXT(())))(\
    __VA_ARGS__, \
    std::declval<__VA_ARGS__>() FIT_EAT \
)
#define FIT_RETURNS_CONSTRUCT FIT_PP_RAIL(FIT_RETURNS_DERAIL_CONSTRUCT)

#endif

#define FIT_AUTO_FORWARD(x) std::forward<decltype(x)>(x)

#endif