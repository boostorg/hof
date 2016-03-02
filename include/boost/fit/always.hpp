/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    always.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FUNCTION_ALWAYS_H
#define BOOST_FIT_GUARD_FUNCTION_ALWAYS_H

#include <boost/fit/detail/unwrap.hpp>
#include <boost/fit/detail/static_const_var.hpp>

/// always
/// ======
/// 
/// Description
/// -----------
/// 
/// The `always` function returns a function object that will always return
/// the value given to it, no matter what parameters are passed to the
/// function object. The `always_ref` version will return a reference, and it
/// requires the value passed in to be an lvalue.
/// 
/// Synopsis
/// --------
/// 
///     template<class T>
///     constexpr /* unspecified */ always(T value);
/// 
///     template<class T>
///     constexpr /* unspecified */ always_ref(T& value);
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
///     int ten = 10;
///     assert( always(ten)(1,2,3,4,5) == 10 );
/// 
///     // Count all
///     template<class Iterator, class T>
///     auto count(Iterator first, Iterator last)
///     {
///         return std::count_if(first, last, always(true));
///     }
/// 


#define BOOST_FIT_NO_CONSTEXPR_VOID 1
#ifndef BOOST_FIT_NO_CONSTEXPR_VOID
#ifdef __clang__
#define BOOST_FIT_NO_CONSTEXPR_VOID 0
#else
#define BOOST_FIT_NO_CONSTEXPR_VOID 1
#endif
#endif

namespace boost { namespace fit { namespace detail {

template<class T>
struct always_base
{
    T x;
    
    constexpr always_base()
    {}
    
    constexpr always_base(T xp) : x(xp)
    {}

    template<class... As>
    constexpr typename unwrap_reference<T>::type 
    operator()(As&&...) const
    {
        return this->x;
    }
};

#if BOOST_FIT_NO_CONSTEXPR_VOID
#define BOOST_FIT_ALWAYS_VOID_RETURN boost::fit::detail::always_base<void>::void_
#else
#define BOOST_FIT_ALWAYS_VOID_RETURN void
#endif

template<>
struct always_base<void>
{
    
    constexpr always_base()
    {}

    struct void_ {};

    template<class... As>
    constexpr BOOST_FIT_ALWAYS_VOID_RETURN 
    operator()(As&&...) const
    {
#if BOOST_FIT_NO_CONSTEXPR_VOID
        return void_();
#endif
    }
};

struct always_f
{
    template<class T>
    constexpr detail::always_base<T> operator()(T x) const
    {
        return detail::always_base<T>(x);
    }

    constexpr detail::always_base<void> operator()() const
    {
        return detail::always_base<void>();
    }
};

struct always_ref_f
{
    template<class T>
    constexpr detail::always_base<T&> operator()(T& x) const
    {
        return detail::always_base<T&>(x);
    }
};

}
BOOST_FIT_DECLARE_STATIC_VAR(always, detail::always_f);
BOOST_FIT_DECLARE_STATIC_VAR(always_ref, detail::always_ref_f);

}} // namespace boost::fit

#endif
