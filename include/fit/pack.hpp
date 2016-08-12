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
///     constexpr auto pack_basic_forward(Ts&&... xs);
/// 
///     // Decay everything before capturing
///     template<class... Ts>
///     constexpr auto pack_decay(Ts&&... xs);
/// 
///     // Join multiple packs together
///     template<class... Ts>
///     constexpr auto pack_join(Ts&&... xs);
/// 
/// Semantics
/// ---------
/// 
///     assert(pack(xs...)(f) == f(xs...));
///     assert(unpack(f)(pack(xs...)) == f(xs...));
/// 
///     assert(pack_join(pack(xs...), pack(ys...)) == pack(xs..., ys...));
/// 
/// 
/// Example
/// -------
/// 
///     #include <fit.hpp>
///     #include <cassert>
///     using namespace fit;
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
///     int main() {
///         int r = pack(3, 2)(sum());
///         assert(r == 5);
///     }
/// 

#include <fit/detail/seq.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/remove_rvalue_reference.hpp>
#include <fit/detail/unwrap.hpp>
#include <fit/detail/static_const_var.hpp>
#include <fit/unpack_sequence.hpp>
#include <fit/returns.hpp>
#include <fit/alias.hpp>
#include <fit/decay.hpp>

namespace fit { namespace detail {

template<class...>
struct pack_tag
{};

template<class T, class Tag>
struct pack_holder
: detail::alias_empty<T, Tag>
{};

template<class Seq, class... Ts>
struct pack_base;

template<class T>
struct is_copyable
: std::integral_constant<bool, (
#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7)
    std::is_constructible<T, typename std::add_const<T>::type>::value ||
#else
    std::is_copy_constructible<T>::value || 
#endif
    std::is_reference<T>::value
)>
{};

template<class T, class Tag, class X, class... Ts, typename std::enable_if<
    is_copyable<T>::value && !std::is_lvalue_reference<T>::value
, int>::type = 0>
constexpr T pack_get(X&& x, Ts&&... xs)
{
    return static_cast<T>(fit::alias_value<Tag, T>(FIT_FORWARD(X)(x), xs...));
}

template<class T, class Tag, class X, class... Ts, typename std::enable_if<
    std::is_lvalue_reference<T>::value
, int>::type = 0>
constexpr T pack_get(X&& x, Ts&&... xs)
{
    return fit::alias_value<Tag, T>(x, xs...);
}

template<class T, class Tag, class X, class... Ts, typename std::enable_if<
    !is_copyable<T>::value
, int>::type = 0>
constexpr auto pack_get(X&& x, Ts&&... xs) FIT_RETURNS
(
    fit::alias_value<Tag, T>(FIT_FORWARD(X)(x), xs...)
);

#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7) || defined(_MSC_VER)
template<class... Ts>
struct pack_holder_base
: Ts::type...
{
    template<class... Xs, class=typename std::enable_if<(sizeof...(Xs) == sizeof...(Ts))>::type>
    constexpr pack_holder_base(Xs&&... xs) 
    : Ts::type(FIT_FORWARD(Xs)(xs))...
    {}
#ifndef _MSC_VER
    // FIT_INHERIT_DEFAULT(pack_holder_base, typename std::remove_cv<typename std::remove_reference<typename Ts::type>::type>::type...)
    FIT_INHERIT_DEFAULT(pack_holder_base, typename Ts::type...)
#endif
};

template<class T>
struct pack_holder_base<T>
: T::type
{
    typedef typename T::type base;
    FIT_INHERIT_CONSTRUCTOR(pack_holder_base, base);
};

template<class... Ts>
struct pack_holder_builder
{
    template<class T, std::size_t N>
    struct apply
    : pack_holder<T, pack_tag<seq<N>, Ts...>>
    {};
};

template<std::size_t... Ns, class... Ts>
struct pack_base<seq<Ns...>, Ts...>
: pack_holder_base<typename pack_holder_builder<Ts...>::template apply<Ts, Ns>...>
{
    typedef pack_holder_base<typename pack_holder_builder<Ts...>::template apply<Ts, Ns>...> base;
    template<class X1, class X2, class... Xs>
    constexpr pack_base(X1&& x1, X2&& x2, Xs&&... xs) 
    : base(FIT_FORWARD(X1)(x1), FIT_FORWARD(X2)(x2), FIT_FORWARD(Xs)(xs)...)
    {}

    template<class X1, typename std::enable_if<(std::is_constructible<base, X1>::value), int>::type = 0>
    constexpr pack_base(X1&& x1) 
    : base(FIT_FORWARD(X1)(x1))
    {}

    // FIT_INHERIT_DEFAULT(pack_base, typename std::remove_cv<typename std::remove_reference<Ts>::type>::type...);
    FIT_INHERIT_DEFAULT(pack_base, Ts...);

    FIT_RETURNS_CLASS(pack_base);
  
    template<class F>
    constexpr auto operator()(F&& f) const FIT_RETURNS
    (
        f(detail::pack_get<Ts, pack_tag<seq<Ns>, Ts...>>(*FIT_CONST_THIS, f)...)
    );

    typedef std::integral_constant<std::size_t, sizeof...(Ts)> fit_function_param_limit;

    template<class F>
    struct apply
    : F::template apply<Ts...>
    {};
};

template<class T>
struct pack_base<seq<0>, T>
: pack_holder_base<pack_holder<T, pack_tag<seq<0>, T>>>
{
    typedef pack_holder_base<pack_holder<T, pack_tag<seq<0>, T>>> base;

    template<class X1, typename std::enable_if<(std::is_constructible<base, X1>::value), int>::type = 0>
    constexpr pack_base(X1&& x1) 
    : base(FIT_FORWARD(X1)(x1))
    {}

    FIT_INHERIT_DEFAULT(pack_base, T);

    FIT_RETURNS_CLASS(pack_base);
  
    template<class F>
    constexpr auto operator()(F&& f) const FIT_RETURNS
    (
        f(detail::pack_get<T, pack_tag<seq<0>, T>>(*FIT_CONST_THIS, f))
    );

    typedef std::integral_constant<std::size_t, 1> fit_function_param_limit;

    template<class F>
    struct apply
    : F::template apply<T>
    {};
};

#else

template<std::size_t... Ns, class... Ts>
struct pack_base<seq<Ns...>, Ts...>
: pack_holder<Ts, pack_tag<seq<Ns>, Ts...>>::type...
{
    // FIT_INHERIT_DEFAULT(pack_base, typename std::remove_cv<typename std::remove_reference<Ts>::type>::type...);
    FIT_INHERIT_DEFAULT(pack_base, Ts...);
    
    template<class... Xs, FIT_ENABLE_IF_CONVERTIBLE_UNPACK(Xs&&, typename pack_holder<Ts, pack_tag<seq<Ns>, Ts...>>::type)>
    constexpr pack_base(Xs&&... xs) : pack_holder<Ts, pack_tag<seq<Ns>, Ts...>>::type(FIT_FORWARD(Xs)(xs))...
    {}
  
    template<class F>
    constexpr auto operator()(F&& f) const FIT_RETURNS
    (
        f(detail::pack_get<Ts, pack_tag<seq<Ns>, Ts...>>(*this, f)...)
    );

    typedef std::integral_constant<std::size_t, sizeof...(Ts)> fit_function_param_limit;

    template<class F>
    struct apply
    : F::template apply<Ts...>
    {};
};

#endif

template<>
struct pack_base<seq<> >
{
    template<class F>
    constexpr auto operator()(F&& f) const FIT_RETURNS
    (f());

    typedef std::integral_constant<std::size_t, 0> fit_function_param_limit;

    template<class F>
    struct apply
    : F::template apply<>
    {};
};

#define FIT_DETAIL_UNPACK_PACK_BASE(ref, move) \
template<class F, std::size_t... Ns, class... Ts> \
constexpr auto unpack_pack_base(F&& f, pack_base<seq<Ns...>, Ts...> ref x) \
FIT_RETURNS(f(fit::alias_value<pack_tag<seq<Ns>, Ts...>, Ts>(move(x), f)...))
FIT_UNARY_PERFECT_FOREACH(FIT_DETAIL_UNPACK_PACK_BASE)

template<class P1, class P2>
struct pack_join_base;

// TODO: Extend to join more than two packs at a time
template<std::size_t... Ns1, class... Ts1, std::size_t... Ns2, class... Ts2>
struct pack_join_base<pack_base<seq<Ns1...>, Ts1...>, pack_base<seq<Ns2...>, Ts2...>>
{
    static constexpr long total_size = sizeof...(Ts1) + sizeof...(Ts2);
    typedef pack_base<typename detail::gens<total_size>::type, Ts1..., Ts2...> result_type;

    template<class P1, class P2>
    static constexpr result_type call(P1&& p1, P2&& p2)
    {
        return result_type(
            detail::pack_get<Ts1, pack_tag<seq<Ns1>, Ts1...>>(FIT_FORWARD(P1)(p1))..., 
            detail::pack_get<Ts2, pack_tag<seq<Ns2>, Ts2...>>(FIT_FORWARD(P2)(p2))...);
    }
};

template<class P1, class P2>
struct pack_join_result 
: pack_join_base<
    typename std::remove_cv<typename std::remove_reference<P1>::type>::type, 
    typename std::remove_cv<typename std::remove_reference<P2>::type>::type
>
{};


struct pack_basic_f
{
    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        pack_base<typename gens<sizeof...(Ts)>::type, typename remove_rvalue_reference<Ts>::type...>(FIT_FORWARD(Ts)(xs)...)
    );
};

struct pack_forward_f
{
    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        pack_base<typename gens<sizeof...(Ts)>::type, Ts&&...>(FIT_FORWARD(Ts)(xs)...)
    );
};

struct pack_decay_f
{
    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        pack_basic_f()(decay(FIT_FORWARD(Ts)(xs))...)
    );
};

template<class P1, class P2>
constexpr typename pack_join_result<P1, P2>::result_type make_pack_join_dual(P1&& p1, P2&& p2)
{
    return pack_join_result<P1, P2>::call(FIT_FORWARD(P1)(p1), FIT_FORWARD(P2)(p2));
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
    return FIT_FORWARD(P1)(p1);
}

template<class P1, class... Ps>
constexpr typename join_type<P1, Ps...>::type make_pack_join(P1&& p1, Ps&&... ps)
{
    return make_pack_join_dual(FIT_FORWARD(P1)(p1), make_pack_join(FIT_FORWARD(Ps)(ps)...));
}

struct pack_join_f
{

    template<class... Ps>
    constexpr auto operator()(Ps&&... ps) const FIT_RETURNS
    (
        make_pack_join(FIT_FORWARD(Ps)(ps)...)
    );
};

}

FIT_DECLARE_STATIC_VAR(pack_basic, detail::pack_basic_f);
FIT_DECLARE_STATIC_VAR(pack_forward, detail::pack_forward_f);
FIT_DECLARE_STATIC_VAR(pack_decay, detail::pack_decay_f);

FIT_DECLARE_STATIC_VAR(pack_join, detail::pack_join_f);

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
