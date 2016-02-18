/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    intrinsics.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_INTRINSICS_HPP
#define FIT_GUARD_INTRINSICS_HPP

#include <type_traits>

#ifdef __clang__
#define FIT_IS_CONSTRUCTIBLE(...) __is_constructible(__VA_ARGS__)
#define FIT_IS_CLASS(...) __is_class(__VA_ARGS__)
#define FIT_IS_EMPTY(...) __is_empty(__VA_ARGS__)
#define FIT_IS_LITERAL(...) __is_literal(__VA_ARGS__)
#define FIT_IS_CONVERTIBLE(...) __is_convertible_to(__VA_ARGS__)
#else
#define FIT_IS_CONSTRUCTIBLE(...) std::is_constructible<__VA_ARGS__>::value
#define FIT_IS_CLASS(...) std::is_class<__VA_ARGS__>::value
#define FIT_IS_EMPTY(...) std::is_empty<__VA_ARGS__>::value
#define FIT_IS_LITERAL(...) std::is_literal<__VA_ARGS__>::value
#define FIT_IS_CONVERTIBLE(...) std::is_convertible_to<__VA_ARGS__>::value
#endif

namespace fit {

} // namespace fit

#endif
