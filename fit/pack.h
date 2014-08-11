/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    pack.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_PACK_H
#define FIT_GUARD_FUNCTION_PACK_H

#include <fit/detail/seq.h>
#include <fit/detail/delegate.h>
#include <fit/detail/remove_rvalue_reference.h>
#include <fit/returns.h>

namespace fit { namespace detail {

struct decay_elem_f
{
    template <class T>
    struct unwrap_reference
    {
        typedef T type;
    };
    template <class T>
    struct unwrap_reference<std::reference_wrapper<T>>
    {
        typedef T& type;
    };

    template<class T>
    constexpr typename unwrap_reference<typename std::decay<T>::type>::type 
    operator()(T&& x) const
    {
        return std::forward<T>(x);
    }
};
static decay_elem_f decay_elem = {};

template<int, class T>
struct pack_holder
{
    T value;

    FIT_DELGATE_CONSTRUCTOR(pack_holder, T, value)
};

template<class Seq, class... Ts>
struct pack_base;


template<int N, class T, class Pack, class... Ts>
constexpr T pack_get(const Pack& p, Ts&&...)
{
    // C style cast(rather than static_cast) is needed for gcc
    return (T)(static_cast<const pack_holder<N, T>&>(p).value);
}


template<int... Ns, class... Ts>
struct pack_base<seq<Ns...>, Ts...>
: pack_holder<Ns, Ts>...
{
    template<class... Xs, FIT_ENABLE_IF_CONVERTIBLE_UNPACK(Xs&&, pack_holder<Ns, Ts>)>
    constexpr pack_base(Xs&&... xs) : pack_holder<Ns, Ts>(std::forward<Xs>(xs))...
    {}

    template<class F>
    constexpr auto operator()(F f) const FIT_RETURNS
    (
        f(pack_get<Ns, Ts>(*this, f)...)
    );
};

template<>
struct pack_base<seq<> >
{
    template<class F>
    constexpr auto operator()(F f) const FIT_RETURNS
    (f());
};

}

template<class... Ts>
constexpr auto pack(Ts&&... xs) FIT_RETURNS
(
    detail::pack_base<typename detail::gens<sizeof...(Ts)>::type, typename detail::remove_rvalue_reference<Ts>::type...>(std::forward<Ts>(xs)...)
);

template<class... Ts>
constexpr auto pack_forward(Ts&&... xs) FIT_RETURNS
(
    detail::pack_base<typename detail::gens<sizeof...(Ts)>::type, Ts&&...>(std::forward<Ts>(xs)...)
);

template<class... Ts>
constexpr auto pack_decay(Ts&&... xs) FIT_RETURNS
(
    pack(detail::decay_elem(std::forward<Ts>(xs))...)
);


}

#endif
