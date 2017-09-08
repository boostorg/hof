/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    is_unpackable.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FIT_GUARD_IS_UNPACKABLE_HPP
#define BOOST_FIT_GUARD_IS_UNPACKABLE_HPP

/// is_unpackable
/// =============
/// 
/// This is a trait that can be used to detect whether the type can be called
/// with `unpack`.
/// 
/// Synopsis
/// --------
/// 
///     template<class T>
///     struct is_unpackable;
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
/// 
///     int main() {
///         static_assert(boost::fit::is_unpackable<std::tuple<int>>::value, "Failed");
///     }
/// 

#include <boost/fit/unpack_sequence.hpp>
#include <boost/fit/is_callable.hpp>
#include <boost/fit/always.hpp>
#include <boost/fit/detail/static_const_var.hpp>
#include <boost/fit/detail/unpack_tuple.hpp>

namespace boost { namespace fit {

namespace detail {

struct unpack_impl_f
{
    template<class F, class Sequence>
    constexpr auto operator()(F&& f, Sequence&& s) const BOOST_FIT_RETURNS
    (
        boost::fit::unpack_sequence<typename std::remove_cv<typename std::remove_reference<Sequence>::type>::type>::
                apply(BOOST_FIT_FORWARD(F)(f), BOOST_FIT_FORWARD(Sequence)(s))
    );
};

BOOST_FIT_DECLARE_STATIC_VAR(unpack_impl, unpack_impl_f);

#if BOOST_FIT_CHECK_UNPACK_SEQUENCE
struct private_unpack_type {};
template<class Sequence>
struct unpack_impl_result
{
    static_assert(boost::fit::is_callable<unpack_impl_f, decltype(boost::fit::always(private_unpack_type())), Sequence>::value,
        "Unpack is invalid for this sequence. The function used to unpack this sequence is not callable."
    );
    typedef decltype(boost::fit::detail::unpack_impl(boost::fit::always(private_unpack_type()), std::declval<Sequence>())) type;
};

template<class Sequence>
struct is_proper_sequence
: std::is_same<
    private_unpack_type, 
    typename unpack_impl_result<Sequence>::type
>
{};
#endif
template<class Sequence, class=void>
struct is_unpackable_impl
: std::true_type
{
#if BOOST_FIT_CHECK_UNPACK_SEQUENCE
    static_assert(is_proper_sequence<Sequence>::value,
        "Unpack is invalid for this sequence. The function used to unpack this sequence does not invoke the function."
    );
#endif
};

template<class Sequence>
struct is_unpackable_impl<Sequence, typename detail::holder<
    typename unpack_sequence<Sequence>::not_unpackable
>::type>
: std::false_type
{};

}

template<class Sequence>
struct is_unpackable
: detail::is_unpackable_impl<
    typename std::remove_cv<typename std::remove_reference<Sequence>::type>::type
>
{
#if BOOST_FIT_CHECK_UNPACK_SEQUENCE
typedef detail::is_unpackable_impl<
    typename std::remove_cv<typename std::remove_reference<Sequence>::type>::type
> base;

typedef std::conditional<base::value, detail::is_proper_sequence<Sequence>, std::true_type> check;
static_assert(check::type::value,
    "Unpack is invalid for this sequence. The function used to unpack this sequence does not invoke the function."
);
#endif
};

}} // namespace boost::fit

#endif
