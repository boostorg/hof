/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    function_param_limit.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_PARAM_LIMIT_HPP
#define FIT_GUARD_FUNCTION_PARAM_LIMIT_HPP

/// function_param_limit
/// ====================
/// 
/// Description
/// -----------
/// 
/// The `function_param_limit` metafunction retrieves the maximum number of
/// parameters for a function. This can be annotated using [`limit`](limit)
/// decorator.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     struct function_param_limit;
/// 
/// See Also
/// --------
/// 
/// * [Partial function evaluation](<Partial function evaluation>)
/// * [limit](limit)
/// 

#include <fit/detail/holder.hpp>
#include <fit/detail/bare.hpp>
#include <type_traits>
#include <cstdint>

namespace fit {

template<class F, class=void>
struct function_param_limit
: std::integral_constant<std::size_t, SIZE_MAX>
{};

template<class F>
struct function_param_limit<F, typename detail::holder<
    typename detail::bare<F>::type::fit_function_param_limit
>::type>
: detail::bare<F>::type::fit_function_param_limit
{};

} // namespace fit

#endif
