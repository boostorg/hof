/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    make.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_MAKE_H
#define FIT_GUARD_MAKE_H

#include <fit/detail/move.hpp>
#include <fit/detail/join.hpp>
#include <fit/detail/delegate.hpp>

namespace fit { namespace detail {

template<template<class...> class Adaptor>
struct make
{
	constexpr make() noexcept
	{}
    template<class... Fs, class Result=FIT_JOIN(Adaptor, Fs...)>
    constexpr Result operator()(Fs... fs) const FIT_NOEXCEPT_CONSTRUCTIBLE(Result, Fs&&...)
    {
        return Result(static_cast<Fs&&>(fs)...);
    }
};

}} // namespace fit

#endif
