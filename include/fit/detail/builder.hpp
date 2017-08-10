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

#define FIT_DECLARE_ADAPTOR(adaptor, ...) \
template<class... Fs> \
struct adaptor ## _adaptor : __VA_ARGS__::template apply<Fs...> \
{ \
    typedef typename __VA_ARGS__::apply<Fs...> builder_base; \
    FIT_INHERIT_CONSTRUCTOR(adaptor ## _adaptor, builder_base) \
}; \
FIT_DECLARE_STATIC_VAR(adaptor, detail::make<adaptor ## _adaptor>);

namespace fit { namespace detail {

}

} // namespace fit

#endif
