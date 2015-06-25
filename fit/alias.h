/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    alias.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_ALIAS_H
#define FIT_GUARD_ALIAS_H

#include <fit/returns.h>
#include <fit/detail/delegate.h>
#include <fit/detail/move.h>
#include <fit/detail/holder.h>

namespace fit {

template<class T>
struct alias_tag;

template<class T, class Tag, class=void>
struct has_tag
: std::false_type
{};

template<class T, class Tag>
struct has_tag<T, Tag, typename detail::holder<
    typename alias_tag<T>::type
>::type>
: std::is_same<typename alias_tag<T>::type, Tag>
{};

namespace detail {

template<class T>
constexpr T& lvalue(T& x)
{
    return x;
}

template<class T>
constexpr const T& lvalue(const T& x)
{
    return x;
}

}

#define FIT_UNARY_PERFECT_ID(...) __VA_ARGS__
#define FIT_UNARY_PERFECT_FOREACH(m) \
    m(const&, fit::detail::lvalue) \
    m(&, fit::detail::lvalue) \
    m(&&, fit::move) \

template<class T, class Tag=void>
struct alias
{
    T value;
    FIT_DELGATE_CONSTRUCTOR(alias, T, value)
};

#define FIT_DETAIL_ALIAS_GET_VALUE(ref, move) \
template<class T, class Tag, class... Ts> \
constexpr auto alias_value(alias<T, Tag> ref a, Ts&&...) FIT_RETURNS(move(a.value))
FIT_UNARY_PERFECT_FOREACH(FIT_DETAIL_ALIAS_GET_VALUE)

template<class T, class Tag>
struct alias_tag<alias<T, Tag>>
{ typedef Tag type; };


template<class T, class Tag=void>
struct alias_inherit 
#if (defined(__GNUC__) && !defined (__clang__))
: std::conditional<(std::is_class<T>::value), T, alias<T>>::type
#else
: T
#endif
{
    FIT_INHERIT_CONSTRUCTOR(alias_inherit, T)
};

#define FIT_DETAIL_ALIAS_INHERIT_GET_VALUE(ref, move) \
template<class T, class Tag, class... Ts, class=typename std::enable_if<(std::is_class<T>::value)>::type> \
constexpr T ref alias_value(alias_inherit<T, Tag> ref a, Ts&&...) \
{ \
    return move(a); \
}
FIT_UNARY_PERFECT_FOREACH(FIT_DETAIL_ALIAS_INHERIT_GET_VALUE)

template<class T, class Tag>
struct alias_tag<alias_inherit<T, Tag>>
{ typedef Tag type; };

template<class T, class Tag=void>
struct alias_construct
{
    template<class... Ts, FIT_ENABLE_IF_CONSTRUCTIBLE(T, Ts...)>
    constexpr alias_construct(Ts&&...)
    {}
};

template<class T, class Tag, class... Ts>
constexpr T alias_value(const alias_construct<T, Tag>&, Ts&&...)
{
    return T();
}

template<class T, class Tag>
struct alias_tag<alias_construct<T, Tag>>
{ typedef Tag type; };

}

#endif
