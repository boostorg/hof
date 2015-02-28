/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    make.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_MAKE_H
#define FIT_GUARD_MAKE_H

#include <fit/detail/move.h>
#include <fit/detail/join.h>

namespace fit { namespace detail {

template<template<class...> class Adaptor>
struct make
{
    template<class... Fs>
    constexpr FIT_JOIN(Adaptor, Fs...) operator()(Fs... fs) const
    {
        return FIT_JOIN(Adaptor, Fs...)(fit::move(fs)...);
    }
};

}}

#endif