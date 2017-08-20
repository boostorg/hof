/*=============================================================================
    Copyright (c) 2017 Paul Fultz II
    builder.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_DETAIL_BUILDER_HPP
#define FIT_GUARD_DETAIL_BUILDER_HPP

#include <fit/detail/delegate.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/callable_base.hpp>
#include <fit/detail/result_of.hpp>
#include <fit/returns.hpp>
#include <fit/always.hpp>
#include <fit/detail/join.hpp>

namespace fit { namespace detail {

template<class T, class... Ts>
struct compute_builder_base
{
    typedef typename T::template apply<Ts...> type;
};

}} // namespace fit
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define FIT_BUILDER_JOIN_BASE_1(...) __VA_ARGS__>::type
#define FIT_BUILDER_JOIN_BASE(...) fit::detail::compute_builder_base<__VA_ARGS__, FIT_BUILDER_JOIN_BASE_1
#else
#define FIT_BUILDER_JOIN_BASE_1(...) <__VA_ARGS__>
#define FIT_BUILDER_JOIN_BASE(...) __VA_ARGS__::template apply FIT_BUILDER_JOIN_BASE_1
#endif

#define FIT_DECLARE_ADAPTOR_USING(adaptor, ...) \
struct adaptor ## _adaptor : __VA_ARGS__ \
{ \
    typedef typename __VA_ARGS__ builder_base; \
    FIT_INHERIT_CONSTRUCTOR(adaptor ## _adaptor, builder_base) \
}; \
FIT_DECLARE_STATIC_VAR(adaptor, detail::make<adaptor ## _adaptor>);

#define FIT_DECLARE_ADAPTOR(adaptor, ...) \
template<class... Fs> \
FIT_DECLARE_ADAPTOR_USING(adaptor, FIT_BUILDER_JOIN_BASE(__VA_ARGS__)(fit::detail::callable_base<Fs>...))

#endif
