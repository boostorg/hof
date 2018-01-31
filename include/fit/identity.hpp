/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    identity.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_IDENTITY_H
#define FIT_GUARD_FUNCTION_IDENTITY_H

/// identity
/// ========
/// 
/// Description
/// -----------
/// 
/// The `identity` function is an unary function object that returns whats given to it. 
/// 
/// Semantics
/// ---------
/// 
///     assert(identity(x) == x);
/// 
/// Synopsis
/// --------
/// 
///     template<class T>
///     constexpr T identity(T&& x);
/// 

#include <utility>
#include <initializer_list>
#include <fit/detail/forward.hpp>
#include <fit/detail/static_const_var.hpp>

namespace fit { namespace identity_detail {

struct identity_base
{
    template<class T>
    constexpr T operator()(T&& x) const 
    noexcept(std::is_reference<T>::value || FIT_IS_NOTHROW_MOVE_CONSTRUCTIBLE(T))
    {
        return FIT_FORWARD(T)(x);
    }

    template<class T>
    constexpr std::initializer_list<T>& operator()(std::initializer_list<T>& x) const noexcept
    {
        return x;
    }

    template<class T>
    constexpr const std::initializer_list<T>& operator()(const std::initializer_list<T>& x) const noexcept
    {
        return x;
    }

    template<class T>
    constexpr std::initializer_list<T> operator()(std::initializer_list<T>&& x) const noexcept(noexcept(std::initializer_list<T>(std::move(x))))
    {
        return FIT_FORWARD(std::initializer_list<T>)(x);
    }
};

}

FIT_DECLARE_STATIC_VAR(identity, identity_detail::identity_base);

} // namespace fit

#endif
