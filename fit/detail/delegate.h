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

#define FIT_ENABLE_IF_CONVERTIBLE(...) \
    typename std::enable_if<std::is_convertible<__VA_ARGS__>::value, int>::type = 0

#define FIT_ENABLE_IF_CONVERTIBLE_UNPACK(...) \
    typename std::enable_if<std::is_convertible<__VA_ARGS__>::value..., int>::type = 0

#define FIT_ENABLE_IF_CONSTRUCTIBLE(...) \
    typename std::enable_if<std::is_constructible<__VA_ARGS__>::value, int>::type = 0

#define FIT_DELGATE_CONSTRUCTOR(C, T, var) \
    template<class... FitXs, FIT_ENABLE_IF_CONSTRUCTIBLE(T, FitXs&&...)> \
    constexpr C(FitXs&&... fit_xs) : var(std::forward<FitXs>(fit_xs)...) {}

// TODO: For compilers that support inheriting constructors replace with
// `using Base::Base;`
#define FIT_INHERIT_CONSTRUCTOR(Derived, Base) FIT_DELGATE_CONSTRUCTOR(Derived, Base, Base)

namespace fit {
namespace detail {

}
}

#endif