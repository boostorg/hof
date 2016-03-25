/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    always.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_ALWAYS_H
#define FIT_GUARD_FUNCTION_ALWAYS_H

#include <fit/detail/unwrap.hpp>
#include <fit/detail/static_const_var.hpp>

/// always
/// ======
/// 
/// Description
/// -----------
/// 
/// The `always` function returns a function object that will always return
/// the value given to it, no matter what parameters are passed to the
/// function object. The nullary version(i.e. `always(void)`) will return
/// `void`. On compilers, that don't support constexpr functions returning
/// `void`, a private empty type is returned instead. This return type is
/// specified as `FIT_ALWAYS_VOID_RETURN`.
/// 
/// Synopsis
/// --------
/// 
///     template<class T>
///     constexpr auto always(T value);
/// 
///     template<class T>
///     constexpr auto always(void);
/// 
/// 
/// Semantics
/// ---------
/// 
///     assert(always(x)(xs...) == x);
/// 
/// Requirements
/// ------------
/// 
/// T must be:
/// 
/// * CopyConstructible
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <algorithm>
///     #include <cassert>
///     using namespace fit;
/// 
///     int main() {
///         int ten = 10;
///         assert( always(ten)(1,2,3,4,5) == 10 );
///     }
/// 
///     // Count all
///     template<class Iterator, class T>
///     auto count(Iterator first, Iterator last)
///     {
///         return std::count_if(first, last, always(true));
///     }
/// 


#ifndef FIT_NO_CONSTEXPR_VOID
#if defined(__clang__) && FIT_HAS_RELAXED_CONSTEXPR
#define FIT_NO_CONSTEXPR_VOID 0
#else
#define FIT_NO_CONSTEXPR_VOID 1
#endif
#endif

namespace fit { namespace always_detail {

template<class T>
struct always_base
{
    T x;
    
    constexpr always_base()
    {}
    
    constexpr always_base(T xp) : x(xp)
    {}

    template<class... As>
    constexpr typename detail::unwrap_reference<T>::type 
    operator()(As&&...) const
    {
        return this->x;
    }
};

#if FIT_NO_CONSTEXPR_VOID
#define FIT_ALWAYS_VOID_RETURN fit::always_detail::always_base<void>::void_
#else
#define FIT_ALWAYS_VOID_RETURN void
#endif

template<>
struct always_base<void>
{
    
    constexpr always_base()
    {}

    struct void_ {};

    template<class... As>
    constexpr FIT_ALWAYS_VOID_RETURN 
    operator()(As&&...) const
    {
#if FIT_NO_CONSTEXPR_VOID
        return void_();
#endif
    }
};

struct always_f
{
    template<class T>
    constexpr always_detail::always_base<T> operator()(T x) const
    {
        return always_detail::always_base<T>(x);
    }

    constexpr always_detail::always_base<void> operator()() const
    {
        return always_detail::always_base<void>();
    }
};

struct always_ref_f
{
    template<class T>
    constexpr always_detail::always_base<T&> operator()(T& x) const
    {
        return always_detail::always_base<T&>(x);
    }
};

}
FIT_DECLARE_STATIC_VAR(always, always_detail::always_f);
FIT_DECLARE_STATIC_VAR(always_ref, always_detail::always_ref_f);

} // namespace fit

#endif
