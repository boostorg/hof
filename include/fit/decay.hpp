/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    decay.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_DECAY_H
#define FIT_GUARD_DECAY_H

/// decay
/// =====
/// 
/// Description
/// -----------
/// 
/// The `decay` function is a unary function object that returns whats given to it after decaying its type.
/// 
/// Synopsis
/// --------
/// 
///     struct
///     {
///         template<class T>
///         constexpr typename decay<T>::type operator()(T&& x) const
///         {
///             return fit::forward<T>(x);
///         }
///     } decay;
/// 
/// References
/// ----------
/// 
/// * [n3255](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2011/n3255.html) - Proposal for `decay_copy`
/// 

#include <fit/detail/delegate.hpp>
#include <fit/detail/unwrap.hpp>
#include <fit/detail/static_const_var.hpp>
#include <fit/detail/forward.hpp>

namespace fit { namespace detail {

template<class T>
struct decay_mf
: unwrap_reference<typename std::decay<T>::type>
{};

struct decay_f
{
    template<
        class T, 
        class Result=typename unwrap_reference<typename std::decay<T>::type>::type, 
        class=typename std::enable_if<(std::is_constructible<Result, T>::value)>::type
    >
    constexpr Result operator()(T&& x) const FIT_NOEXCEPT_CONSTRUCTIBLE(Result, T&&)
    {
        return FIT_FORWARD(T)(x);
    }
};

}

FIT_DECLARE_STATIC_VAR(decay, detail::decay_f);

} // namespace fit

#endif
