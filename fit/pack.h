/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    pack.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_PACK_H
#define FIT_GUARD_FUNCTION_PACK_H

/// pack
/// ====
/// 
/// Description
/// -----------
/// 
/// The `pack` function returns a higher order function object that takes a
/// function that will be passed the initial elements. The function object is
/// a sequence that can be unpacked with `unpack_adaptor` as well. Also,
/// `pack_join` can be used to join multiple packs together.
/// 
/// Synopsis
/// --------
/// 
///     // Capture lvalues by reference and rvalues by value.
///     template<class... Ts>
///     constexpr auto pack(Ts&&... xs);
/// 
///     // Capture lvalues by reference and rvalue reference by reference
///     template<class... Ts>
///     constexpr auto pack_perfect(Ts&&... xs);
/// 
///     // Decay everything before capturing
///     template<class... Ts>
///     constexpr auto pack_decay(Ts&&... xs);
/// 
///     // Join multiple packs together
///     template<class... Ts>
///     constexpr auto pack_join(Ts&&... xs);
/// 
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
///     int r = pack(3, 2)(sum());
///     assert(r == 5);
/// 

#include <fit/detail/seq.h>
#include <fit/detail/delegate.h>
#include <fit/detail/remove_rvalue_reference.h>
#include <fit/detail/unwrap.h>
#include <fit/detail/static_const_var.h>
#include <fit/returns.h>

#ifndef FIT_HAS_RVALUE_THIS
#define FIT_HAS_RVALUE_THIS 1
#endif

#ifndef FIT_PACK_HAS_EBO
#ifdef __clang__
#define FIT_PACK_HAS_EBO 1
#else
#define FIT_PACK_HAS_EBO 0
#endif
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
#if FIT_PACK_HAS_EBO
template<int N, class T, class Tag>
struct pack_holder<N, T, Tag, typename std::enable_if<(std::is_empty<T>::value)>::type>
: T
{
    constexpr const T& get_value() const
    {
        return *this;
    }

    FIT_INHERIT_CONSTRUCTOR(pack_holder, T)
};
#endif
template<class Seq, class... Ts>
struct pack_base;


template<int N, class T, class Tag, class... Ts>
constexpr T&& pack_get(const pack_holder<N, T, Tag>& p, Ts&&...)
{
    // C style cast(rather than static_cast) is needed for gcc
    return (T&&)(p.get_value());
}

#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7) || defined(_MSC_VER)
template<class Seq, class... Ts>
struct pack_holder_base;

template<int... Ns, class... Ts>
struct pack_holder_base<seq<Ns...>, Ts...>
: pack_holder<Ns, Ts, pack_tag<Ts...>>...
{
    template<class... Xs>
    constexpr pack_holder_base(Xs&&... xs) : pack_holder<Ns, Ts, pack_tag<Ts...>>(fit::forward<Xs>(xs))...
    {}
};

template<int... Ns, class... Ts>
struct pack_base<seq<Ns...>, Ts...>
: pack_holder_base<seq<Ns...>, Ts...>
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
: pack_holder<Ns, Ts, pack_tag<Ts...>>...
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
struct pack_join_result 
: pack_join_base<
    typename std::remove_cv<typename std::remove_reference<P1>::type>::type, 
    typename std::remove_cv<typename std::remove_reference<P2>::type>::type
>
{};


struct pack_f
{
    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        pack_base<typename gens<sizeof...(Ts)>::type, typename remove_rvalue_reference<Ts>::type...>(fit::forward<Ts>(xs)...)
    );
};

struct pack_forward_f
{
    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        pack_base<typename gens<sizeof...(Ts)>::type, Ts&&...>(fit::forward<Ts>(xs)...)
    );
};

struct pack_decay_f
{
    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        pack_f()(decay_elem(fit::forward<Ts>(xs))...)
    );
};

template<class P1, class P2>
constexpr typename pack_join_result<P1, P2>::result_type make_pack_join_dual(P1&& p1, P2&& p2)
{
    return pack_join_result<P1, P2>::call(fit::forward<P1>(p1), fit::forward<P2>(p2));
}

// Manually compute join return type to make older gcc happy
template<class... Ts>
struct join_type;

template<class T>
struct join_type<T>
{
    typedef T type;
};

template<class T, class... Ts>
struct join_type<T, Ts...>
{
    typedef typename pack_join_result<T, typename join_type<Ts...>::type>::result_type type;
};

template<class P1>
constexpr P1 make_pack_join(P1&& p1)
{
    return fit::forward<P1>(p1);
}

template<class P1, class... Ps>
constexpr typename join_type<P1, Ps...>::type make_pack_join(P1&& p1, Ps&&... ps)
{
    return make_pack_join_dual(fit::forward<P1>(p1), make_pack_join(fit::forward<Ps>(ps)...));
}

struct pack_join_f
{

    template<class... Ps>
    constexpr auto operator()(Ps&&... ps) const FIT_RETURNS
    (
        make_pack_join(fit::forward<Ps>(ps)...)
    );
};

}

FIT_DECLARE_STATIC_VAR(pack, detail::pack_f);
FIT_DECLARE_STATIC_VAR(pack_forward, detail::pack_forward_f);
FIT_DECLARE_STATIC_VAR(pack_decay, detail::pack_decay_f);

FIT_DECLARE_STATIC_VAR(pack_join, detail::pack_join_f);

}

#endif
