/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    make.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_MAKE_H
#define BOOST_FIT_GUARD_MAKE_H

#include <boost/fit/detail/move.hpp>
#include <boost/fit/detail/join.hpp>
#include <boost/fit/detail/delegate.hpp>

namespace boost { namespace fit { namespace detail {

template<template<class...> class Adaptor>
struct make
{
	constexpr make() noexcept
	{}
    template<class... Fs, class Result=BOOST_FIT_JOIN(Adaptor, Fs...)>
    constexpr Result operator()(Fs... fs) const BOOST_FIT_NOEXCEPT_CONSTRUCTIBLE(Result, Fs&&...)
    {
        return Result(static_cast<Fs&&>(fs)...);
    }
};

}}} // namespace boost::fit

#endif
