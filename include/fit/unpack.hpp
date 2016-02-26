/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    unpack.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_UNPACK_H
#define FIT_GUARD_UNPACK_H

/// unpack
/// ======
/// 
/// Description
/// -----------
/// 
/// The `unpack` function adaptor takes a sequence and uses the elements of
/// the sequence for the arguments to the function. Multiple sequences can be
/// passed to the function. All elements from each sequence will be passed
/// into the function. 
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     unpack_adaptor<F> unpack(F f);
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [Callable](concepts.md#callable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     struct sum
///     {
///         template<class T, class U>
///         T operator()(T x, U y) const
///         {
///             return x+y;
///         }
///     };
/// 
///     int r = unpack(sum())(std::make_tuple(3,2));
///     assert(r == 5);
/// 
/// 
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
///     static_assert(fit::is_unpackable<std::tuple<int>>::value, "Failed");
/// 
/// unpack_sequence
/// ===============
/// 
/// How to unpack a sequence can be defined by specializing `unpack_sequence`.
/// By default, `std::tuple` can be used with unpack.
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
///     }
/// 

#include <fit/returns.hpp>
#include <tuple>
#include <fit/detail/seq.hpp>
#include <fit/capture.hpp>
#include <fit/always.hpp>
#include <fit/reveal.hpp>
#include <fit/detail/and.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/holder.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/make.hpp>
#include <fit/detail/static_const_var.hpp>

#ifndef FIT_CHECK_UNPACK_SEQUENCE
#define FIT_CHECK_UNPACK_SEQUENCE 1
#endif


namespace fit {

template<class Sequence, class=void>
struct unpack_sequence
{
    typedef void not_unpackable;
};


namespace detail {

struct unpack_impl_f
{
    template<class F, class Sequence>
    constexpr auto operator()(F&& f, Sequence&& s) const FIT_RETURNS
    (
        fit::unpack_sequence<typename std::remove_cv<typename std::remove_reference<Sequence>::type>::type>::
                apply(FIT_FORWARD(F)(f), FIT_FORWARD(Sequence)(s))
    );
};

FIT_DECLARE_STATIC_VAR(unpack_impl, unpack_impl_f);

struct private_unpack_type {};

#if FIT_CHECK_UNPACK_SEQUENCE
template<class Sequence>
struct unpack_impl_result
{
    static_assert(fit::is_callable<unpack_impl_f, decltype(always(private_unpack_type())), Sequence>::value,
        "Unpack is invalid for this sequence. The function used to unpack this sequence is not callable."
    );
    typedef decltype(unpack_impl(always(private_unpack_type()), std::declval<Sequence>())) type;
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
#if FIT_CHECK_UNPACK_SEQUENCE
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

template<class F, class Sequence>
constexpr auto unpack_simple(F&& f, Sequence&& s) FIT_RETURNS
(
    unpack_impl(FIT_FORWARD(F)(f), FIT_FORWARD(Sequence)(s))
)

template<class F, class... Sequences>
constexpr auto unpack_join(F&& f, Sequences&&... s) FIT_RETURNS
(
    fit::pack_join(unpack_simple(fit::pack_forward, FIT_FORWARD(Sequences)(s))...)(FIT_FORWARD(F)(f))
);

}

template<class Sequence>
struct is_unpackable
: detail::is_unpackable_impl<
    typename std::remove_cv<typename std::remove_reference<Sequence>::type>::type
>
{
#if FIT_CHECK_UNPACK_SEQUENCE
typedef detail::is_unpackable_impl<
    typename std::remove_cv<typename std::remove_reference<Sequence>::type>::type
> base;

typedef std::conditional<base::value, detail::is_proper_sequence<Sequence>, std::true_type> check;
static_assert(check::type::value,
    "Unpack is invalid for this sequence. The function used to unpack this sequence does not invoke the function."
);
#endif
};

template<class F>
struct unpack_adaptor : detail::callable_base<F>
{
    typedef unpack_adaptor fit_rewritable1_tag;
    FIT_INHERIT_CONSTRUCTOR(unpack_adaptor, detail::callable_base<F>);

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    struct unpack_failure
    {
        template<class Failure>
        struct apply
        {
            struct deducer
            {
                template<class... Ts>
                typename Failure::template of<Ts...> operator()(Ts&&...) const;
            };

            template<class T, class=typename std::enable_if<(
                is_unpackable<T>::value
            )>::type>
            static auto deduce(T&& x)
            FIT_RETURNS
            (
                detail::unpack_simple(deducer(), FIT_FORWARD(T)(x))
            );

            template<class T, class... Ts, class=typename std::enable_if<(
                is_unpackable<T>::value && FIT_AND_UNPACK(is_unpackable<Ts>::value)
            )>::type>
            static auto deduce(T&& x, Ts&&... xs) FIT_RETURNS
            (
                detail::unpack_join(deducer(), FIT_FORWARD(T)(x), FIT_FORWARD(Ts)(xs)...)
            );

            template<class... Ts>
            struct of
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
            : std::enable_if<true, decltype(apply::deduce(std::declval<Ts>()...))>::type
#else
            : decltype(apply::deduce(std::declval<Ts>()...))
#endif
            {};
        };
    };

    struct failure
    : failure_map<unpack_failure, detail::callable_base<F>>
    {};

    FIT_RETURNS_CLASS(unpack_adaptor);
    template<class T, class=typename std::enable_if<(
        is_unpackable<T>::value
    )>::type>
    constexpr auto operator()(T&& x) const
    FIT_RETURNS
    (
        detail::unpack_simple(FIT_MANGLE_CAST(const detail::callable_base<F>&)(FIT_CONST_THIS->base_function(x)), FIT_FORWARD(T)(x))
    );

    template<class T, class... Ts, class=typename std::enable_if<(
        is_unpackable<T>::value && FIT_AND_UNPACK(is_unpackable<Ts>::value)
    )>::type>
    constexpr auto operator()(T&& x, Ts&&... xs) const FIT_RETURNS
    (
        detail::unpack_join(FIT_MANGLE_CAST(const detail::callable_base<F>&)(FIT_CONST_THIS->base_function(x)), FIT_FORWARD(T)(x), FIT_FORWARD(Ts)(xs)...)
    );
};

FIT_DECLARE_STATIC_VAR(unpack, detail::make<unpack_adaptor>);

namespace detail {

template<class Sequence>
constexpr typename gens<std::tuple_size<Sequence>::value>::type 
make_tuple_gens(const Sequence&)
{
    return {};
}

// template<class F, class... Ts, std::size_t ...N>
// constexpr auto unpack_tuple(F&& f, std::tuple<Ts...> && t, seq<N...>) FIT_RETURNS
// (
//     f(FIT_FORWARD(Ts)(std::get<N>(t))...)
// );

// template<class F, class... Ts, std::size_t ...N>
// constexpr auto unpack_tuple(F&& f, std::tuple<Ts...> & t, seq<N...>) FIT_RETURNS
// (
//     // f(FIT_FORWARD(Ts)(std::get<N>(t))...)
//     f(std::get<N>(t)...)
// );

// template<class F, class... Ts, std::size_t ...N>
// constexpr auto unpack_tuple(F&& f, const std::tuple<Ts...> & t, seq<N...>) FIT_RETURNS
// (
//     f(std::get<N>(t)...)
// );

template<class F, class T, std::size_t ...N>
constexpr auto unpack_tuple(F&& f, T&& t, seq<N...>) FIT_RETURNS
(
    f(
        FIT_AUTO_FORWARD(std::get<N>(FIT_AUTO_FORWARD(t)))...
    )
);

}

template<class... Ts>
struct unpack_sequence<std::tuple<Ts...>>
{
    template<class F, class S>
    constexpr static auto apply(F&& f, S&& t) FIT_RETURNS
    (
        detail::unpack_tuple(FIT_FORWARD(F)(f), FIT_FORWARD(S)(t), detail::make_tuple_gens(t))
    );

    // template<class F, class S>
    // constexpr static auto apply(F&& f, S&& t)
    // {
    //     return detail::unpack_tuple(FIT_FORWARD(F)(f), t, detail::make_tuple_gens(t));
    // }
};

template<class T, class... Ts>
struct unpack_sequence<detail::pack_base<T, Ts...>>
{
    template<class F, class P>
    constexpr static auto apply(F&& f, P&& p) FIT_RETURNS
    (
        fit::detail::unpack_pack_base(FIT_FORWARD(F)(f), FIT_FORWARD(P)(p))
    );
};

} // namespace fit

#endif
