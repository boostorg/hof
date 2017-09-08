/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    delgate.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_FUNCTION_DELGATE_H
#define BOOST_FIT_GUARD_FUNCTION_DELGATE_H

#include <type_traits>
#include <utility>
#include <boost/fit/config.hpp>
#include <boost/fit/detail/and.hpp>
#include <boost/fit/detail/holder.hpp>
#include <boost/fit/detail/forward.hpp>
#include <boost/fit/detail/using.hpp>
#include <boost/fit/detail/intrinsics.hpp>
#include <boost/fit/detail/noexcept.hpp>


#define BOOST_FIT_ENABLE_IF_CONVERTIBLE(...) \
    class=typename std::enable_if<BOOST_FIT_IS_CONVERTIBLE(__VA_ARGS__)>::type

#define BOOST_FIT_ENABLE_IF_CONVERTIBLE_UNPACK(...) \
    class=typename std::enable_if<BOOST_FIT_AND_UNPACK(BOOST_FIT_IS_CONVERTIBLE(__VA_ARGS__))>::type

#define BOOST_FIT_ENABLE_IF_BASE_OF(...) \
    class=typename std::enable_if<BOOST_FIT_IS_BASE_OF(__VA_ARGS__)>::type

#define BOOST_FIT_ENABLE_IF_CONSTRUCTIBLE(...) \
    class=typename std::enable_if<BOOST_FIT_IS_CONSTRUCTIBLE(__VA_ARGS__)>::type

#define BOOST_FIT_NOEXCEPT_CONSTRUCTIBLE(...) \
    BOOST_FIT_NOEXCEPT(BOOST_FIT_IS_NOTHROW_CONSTRUCTIBLE(__VA_ARGS__))

#define BOOST_FIT_INHERIT_DEFAULT(C, ...) \
    template<bool FitPrivateEnableBool_##__LINE__=true, \
    class=typename std::enable_if<FitPrivateEnableBool_##__LINE__ && boost::fit::detail::is_default_constructible_c<__VA_ARGS__>()>::type> \
    constexpr C() BOOST_FIT_NOEXCEPT(boost::fit::detail::is_nothrow_default_constructible_c<__VA_ARGS__>()) {}

#define BOOST_FIT_INHERIT_DEFAULT_EMPTY(C, ...) \
    template<bool FitPrivateEnableBool_##__LINE__=true, \
    class=typename std::enable_if<FitPrivateEnableBool_##__LINE__ && \
        boost::fit::detail::is_default_constructible_c<__VA_ARGS__>() && BOOST_FIT_IS_EMPTY(__VA_ARGS__) \
    >::type> \
    constexpr C() BOOST_FIT_NOEXCEPT(boost::fit::detail::is_nothrow_default_constructible_c<__VA_ARGS__>()) {}

#if BOOST_FIT_NO_TYPE_PACK_EXPANSION_IN_TEMPLATE

#define BOOST_FIT_DELGATE_PRIMITIVE_CONSTRUCTOR(constexpr_, C, T, var) \
    template<class... FitXs, typename boost::fit::detail::enable_if_constructible<C, T, FitXs...>::type = 0> \
    constexpr_ C(FitXs&&... fit_xs) \
    BOOST_FIT_NOEXCEPT_CONSTRUCTIBLE(T, FitXs&&...) \
    : var((FitXs&&)boost::fit::forward<FitXs>(fit_xs)...) {}
    
#else
#define BOOST_FIT_DELGATE_PRIMITIVE_CONSTRUCTOR(constexpr_, C, T, var) \
    template<class... FitXs, BOOST_FIT_ENABLE_IF_CONSTRUCTIBLE(T, FitXs&&...)> \
    constexpr_ C(FitXs&&... fit_xs) \
    BOOST_FIT_NOEXCEPT_CONSTRUCTIBLE(T, FitXs&&...) \
    : var(BOOST_FIT_FORWARD(FitXs)(fit_xs)...) {}

#endif

#define BOOST_FIT_DELGATE_CONSTRUCTOR(C, T, var) BOOST_FIT_DELGATE_PRIMITIVE_CONSTRUCTOR(constexpr, C, T, var)

// Currently its faster to use `BOOST_FIT_DELGATE_CONSTRUCTOR` than `using
// Base::Base;`
#if 1
#define BOOST_FIT_INHERIT_CONSTRUCTOR(Derived, Base) BOOST_FIT_DELGATE_CONSTRUCTOR(Derived, Base, Base)
#else
#define BOOST_FIT_INHERIT_CONSTRUCTOR(Derived, Base) \
    using fit_inherit_base = Base; \
    using fit_inherit_base::fit_inherit_base; \
    Derived()=default; \
    template<class FitX, BOOST_FIT_ENABLE_IF_CONVERTIBLE(FitX, Base)> \
    constexpr Derived(FitX&& fit_x) : Base(BOOST_FIT_FORWARD(FitX)(fit_x)) {}
#endif

namespace boost { namespace fit {
namespace detail {

template<class... Xs>
constexpr bool is_nothrow_default_constructible_c()
{
    return BOOST_FIT_AND_UNPACK(BOOST_FIT_IS_NOTHROW_CONSTRUCTIBLE(Xs));
}

template<class... Xs>
constexpr bool is_default_constructible_c()
{
    return BOOST_FIT_AND_UNPACK(BOOST_FIT_IS_DEFAULT_CONSTRUCTIBLE(Xs));
}

template<class... Xs>
BOOST_FIT_USING(is_default_constructible, std::integral_constant<bool, is_default_constructible_c<Xs...>()>);

template<class C, class X, class... Xs>
struct enable_if_constructible
: std::enable_if<is_constructible<X, Xs&&...>::value, int>
{};

}
}} // namespace boost::fit

#endif
