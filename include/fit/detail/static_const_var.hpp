/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    static_const_var.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_STATIC_CONST_H
#define FIT_GUARD_STATIC_CONST_H

#include <fit/detail/inline_var.hpp>

namespace fit { namespace detail {

template<class T>
struct static_const_storage
{
    static constexpr T value = T();
};

template<class T>
constexpr T static_const_storage<T>::value;

struct static_const_var_factory
{
    constexpr static_const_var_factory()
    {}

    template<class T>
    constexpr const T& operator=(const T&) const
    {
        // static_assert(FIT_IS_DEFAULT_CONSTRUCTIBLE(T), "Static const variable must be default constructible");
        return static_const_storage<T>::value;
    }
};
}

template<class T>
constexpr const T& static_const_var()
{
    return detail::static_const_storage<T>::value;
}


} // namespace fit

#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define FIT_STATIC_AUTO_REF static auto&
#else
#define FIT_STATIC_AUTO_REF static constexpr auto&
#endif

#if FIT_HAS_INLINE_VAR
#define FIT_STATIC_CONST_VAR(name) FIT_INLINE_CONST_VAR(name)
#else
#define FIT_STATIC_CONST_VAR(name) static constexpr auto& name = fit::detail::static_const_var_factory()
#endif

#define FIT_DECLARE_STATIC_VAR(name, ...) FIT_STATIC_CONST_VAR(name) = __VA_ARGS__{}

#endif
