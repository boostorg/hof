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
#include <fit/detail/unwrap.h>
#include <fit/returns.h>

#ifndef FIT_HAS_RVALUE_THIS
#define FIT_HAS_RVALUE_THIS 1
#endif

namespace fit { namespace detail {

struct decay_elem_f
{
    template<class T>
    constexpr typename unwrap_reference<typename std::decay<T>::type>::type 
    operator()(T&& x) const
    {
        return fit::forward<T>(x);
    }
};
static decay_elem_f decay_elem = {};

template<class...>
struct pack_tag
{};

template<int, class T, class, class=void>
struct pack_holder
{
    T value;

    constexpr const T& get_value() const
    {
        return this->value;
    }

    FIT_DELGATE_CONSTRUCTOR(pack_holder, T, value)
};

template<int N, class T, class Tag>
struct pack_holder<N, T, Tag, typename std::enable_if<(std::is_empty<T>::value)>::type>
: private T
{
    constexpr const T& get_value() const
    {
        return *this;
    }

    FIT_INHERIT_CONSTRUCTOR(pack_holder, T)
};

template<class Seq, class... Ts>
struct pack_base;


template<int N, class T, class Tag, class... Ts>
constexpr T&& pack_get(const pack_holder<N, T, Tag>& p, Ts&&...)
{
    // C style cast(rather than static_cast) is needed for gcc
    return (T&&)(p.get_value());
}

#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
template<class Seq, class... Ts>
struct pack_holder_base;

template<int... Ns, class... Ts>
struct pack_holder_base<seq<Ns...>, Ts...>
: private pack_holder<Ns, Ts, pack_tag<Ts...>>...
{
    template<class... Xs>
    constexpr pack_holder_base(Xs&&... xs) : pack_holder<Ns, Ts, pack_tag<Ts...>>(fit::forward<Xs>(xs))...
    {}
};

template<int... Ns, class... Ts>
struct pack_base<seq<Ns...>, Ts...>
: private pack_holder_base<seq<Ns...>, Ts...>
{
    typedef pack_holder_base<seq<Ns...>, Ts...> base;
    template<class X1, class X2, class... Xs>
    constexpr pack_base(X1&& x1, X2&& x2, Xs&&... xs) 
    : base(fit::forward<X1>(x1), fit::forward<X2>(x2), fit::forward<Xs>(xs)...)
    {}

    template<class X1, typename std::enable_if<(!std::is_convertible<X1, pack_base>::value), int>::type = 0>
    constexpr pack_base(X1&& x1) 
    : base(fit::forward<X1>(x1))
    {}

    constexpr pack_base()
    {}

    FIT_RETURNS_CLASS(pack_base);
  
    template<class F>
    constexpr auto operator()(F&& f) const FIT_RETURNS
    (
        f(pack_get<Ns, Ts, pack_tag<Ts...>>(*FIT_CONST_THIS, f)...)
    );
};

#else

template<int... Ns, class... Ts>
struct pack_base<seq<Ns...>, Ts...>
: private pack_holder<Ns, Ts, pack_tag<Ts...>>...
{
    template<class... Xs, FIT_ENABLE_IF_CONVERTIBLE_UNPACK(Xs&&, pack_holder<Ns, Ts, pack_tag<Ts...>>)>
    constexpr pack_base(Xs&&... xs) : pack_holder<Ns, Ts, pack_tag<Ts...>>(fit::forward<Xs>(xs))...
    {}
  
    template<class F>
    constexpr auto operator()(F&& f) const FIT_RETURNS
    (
        f(pack_get<Ns, Ts, pack_tag<Ts...>>(*this, f)...)
    );
};

#endif

template<>
struct pack_base<seq<> >
{
    template<class F>
    constexpr auto operator()(F&& f) const FIT_RETURNS
    (f());
};

template<class P1, class P2>
struct pack_join_base;

template<int... Ns1, class... Ts1, int... Ns2, class... Ts2>
struct pack_join_base<pack_base<seq<Ns1...>, Ts1...>, pack_base<seq<Ns2...>, Ts2...>>
{
    static constexpr long total_size = sizeof...(Ts1) + sizeof...(Ts2);
    typedef pack_base<typename detail::gens<total_size>::type, Ts1..., Ts2...> result_type;

    template<class P1, class P2>
    static constexpr result_type call(P1&& p1, P2&& p2)
    {
        // TODO: static_assert that the pack is an rvalue if its only moveable
        return result_type(
            pack_get<Ns1, Ts1>(fit::forward<P1>(p1))..., 
            pack_get<Ns2, Ts2>(fit::forward<P2>(p2))...);
    }
};

template<class P1, class P2>
struct pack_join 
: pack_join_base<
    typename std::remove_cv<typename std::remove_reference<P1>::type>::type, 
    typename std::remove_cv<typename std::remove_reference<P2>::type>::type
>
{};

}

template<class... Ts>
constexpr auto pack(Ts&&... xs) FIT_RETURNS
(
    detail::pack_base<typename detail::gens<sizeof...(Ts)>::type, typename detail::remove_rvalue_reference<Ts>::type...>(fit::forward<Ts>(xs)...)
);

template<class... Ts>
constexpr auto pack_forward(Ts&&... xs) FIT_RETURNS
(
    detail::pack_base<typename detail::gens<sizeof...(Ts)>::type, Ts&&...>(fit::forward<Ts>(xs)...)
);

template<class... Ts>
constexpr auto pack_decay(Ts&&... xs) FIT_RETURNS
(
    pack(detail::decay_elem(fit::forward<Ts>(xs))...)
);

template<class P1, class P2>
constexpr typename detail::pack_join<P1, P2>::result_type pack_join(P1&& p1, P2&& p2)
{
    return detail::pack_join<P1, P2>::call(fit::forward<P1>(p1), fit::forward<P2>(p2));
}


}

#endif
