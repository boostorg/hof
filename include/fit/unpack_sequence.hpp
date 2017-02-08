/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    unpack_sequence.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_UNPACK_SEQUENCE_HPP
#define FIT_GUARD_UNPACK_SEQUENCE_HPP

/// unpack_sequence
/// ===============
/// 
/// How to unpack a sequence can be defined by specializing `unpack_sequence`.
/// By default, `std::tuple` is already specialized.
/// 
/// Synopsis
/// --------
/// 
///     template<class Sequence, class=void>
///     struct unpack_sequence;
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
/// 
///     template<class... Ts>
///     struct my_sequence;
/// 
///     namespace fit {
///         template<class... Ts>
///         struct unpack_sequence<my_sequence<Ts...>>
///         {
///             template<class F, class Sequence>
///             constexpr static auto apply(F&& f, Sequence&& s) FIT_RETURNS
///             (
///                 s(std::forward<F>(f))
///             );
///         };
///     } // namespace fit
/// 
///     int main() {
///     }
/// 
/// See Also
/// --------
/// 
/// * [unpack](unpack)
/// 

#include <fit/config.hpp>

namespace fit {

template<class Sequence, class=void>
struct unpack_sequence
{
    typedef void not_unpackable;
};

} // namespace fit

#endif
