/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    delgate.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_DELGATE_H
#define FIT_GUARD_FUNCTION_DELGATE_H

#include <type_traits>
#include <utility>
#include <fit/config.hpp>
#include <fit/detail/and.hpp>
#include <fit/detail/holder.hpp>
#include <fit/detail/forward.hpp>
#include <fit/detail/using.hpp>
#include <fit/detail/intrinsics.hpp>


#define FIT_ENABLE_IF_CONVERTIBLE(...) \
    class=typename std::enable_if<FIT_IS_CONVERTIBLE(__VA_ARGS__)>::type

#define FIT_ENABLE_IF_CONVERTIBLE_UNPACK(...) \
    class=typename std::enable_if<FIT_AND_UNPACK(FIT_IS_CONVERTIBLE(__VA_ARGS__))>::type

#define FIT_ENABLE_IF_BASE_OF(...) \
    class=typename std::enable_if<FIT_IS_BASE_OF(__VA_ARGS__)>::type

#define FIT_ENABLE_IF_CONSTRUCTIBLE(...) \
    class=typename std::enable_if<FIT_IS_CONSTRUCTIBLE(__VA_ARGS__)>::type

#define FIT_INHERIT_DEFAULT(C, ...) \
    template<bool FitPrivateEnableBool_##__LINE__=true, \
    class=typename std::enable_if<FitPrivateEnableBool_##__LINE__ && fit::detail::is_default_constructible_c<__VA_ARGS__>()>::type> \
    constexpr C() {}

#define FIT_INHERIT_DEFAULT_EMPTY(C, ...) \
    template<bool FitPrivateEnableBool_##__LINE__=true, \
    class=typename std::enable_if<FitPrivateEnableBool_##__LINE__ && \
        fit::detail::is_default_constructible_c<__VA_ARGS__>() && FIT_IS_EMPTY(__VA_ARGS__) \
    >::type> \
    constexpr C() {}

#if FIT_NO_TYPE_PACK_EXPANSION_IN_TEMPLATE

#define FIT_DELGATE_PRIMITIVE_CONSTRUCTOR(constexpr_, C, T, var) \
    template<class... FitXs, typename fit::detail::enable_if_constructible<C, T, FitXs...>::type = 0> \
    constexpr_ C(FitXs&&... fit_xs) : var((FitXs&&)fit::forward<FitXs>(fit_xs)...) {}
    
#else
#define FIT_DELGATE_PRIMITIVE_CONSTRUCTOR(constexpr_, C, T, var) \
    template<class... FitXs, FIT_ENABLE_IF_CONSTRUCTIBLE(T, FitXs&&...)> \
    constexpr_ C(FitXs&&... fit_xs) : var(FIT_FORWARD(FitXs)(fit_xs)...) {}

#endif

#define FIT_DELGATE_CONSTRUCTOR(C, T, var) FIT_DELGATE_PRIMITIVE_CONSTRUCTOR(constexpr, C, T, var)

// Currently its faster to use `FIT_DELGATE_CONSTRUCTOR` than `using
// Base::Base;`
#if 1
#define FIT_INHERIT_CONSTRUCTOR(Derived, Base) FIT_DELGATE_CONSTRUCTOR(Derived, Base, Base)
#else
#define FIT_INHERIT_CONSTRUCTOR(Derived, Base) \
    using fit_inherit_base = Base; \
    using fit_inherit_base::fit_inherit_base; \
    Derived()=default; \
    template<class FitX, FIT_ENABLE_IF_CONVERTIBLE(FitX, Base)> \
    constexpr Derived(FitX&& fit_x) : Base(FIT_FORWARD(FitX)(fit_x)) {}
#endif

namespace fit {
namespace detail {

template<class... Xs>
constexpr bool is_default_constructible_c()
{
    return FIT_AND_UNPACK(FIT_IS_DEFAULT_CONSTRUCTIBLE(Xs));
}

template<class... Xs>
FIT_USING(is_default_constructible, std::integral_constant<bool, is_default_constructible_c<Xs...>()>);

template<class C, class X, class... Xs>
struct enable_if_constructible
: std::enable_if<is_constructible<X, Xs&&...>::value, int>
{};

}
} // namespace fit

#endif
