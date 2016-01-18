/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    alias.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_ALIAS_H
#define FIT_GUARD_ALIAS_H

#include <fit/returns.hpp>
#include <fit/detail/delegate.hpp>
#include <fit/detail/move.hpp>
#include <fit/detail/holder.hpp>

/// alias
/// =====
/// 
/// Description
/// -----------
/// 
/// The `alias` class wraps a type with a new type that can be tagged by the
/// user. There are three different ways the value can be stored: as a member
/// variable, by inheritance, or as a static member variable. The value can
/// be retrieved uniformily using the `alias_value` function.
/// 
/// Synopsis
/// --------
/// 
///     // Alias the type using a member variable
///     template<class T, class Tag=void>
///     class alias;
/// 
///     // Alias the type by inheriting
///     template<class T, class Tag=void>
///     class alias_inherit;
/// 
///     // Alias the type using a static variable
///     template<class T, class Tag=void>
///     class alias_static;
/// 
///     // Retrieve tag from alias
///     template<class Alias>
///     class alias_tag;
/// 
///     // Check if type has a certian tag
///     template<class T, class Tag>
///     class has_tag;
/// 
///     // Retrieve value from alias
///     template<class Alias>
///     constexpr auto alias_value(Alias&&);
/// 

#ifdef _MSC_VER
#pragma warning(disable: 4579)
#endif

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
template<class Tag, class T, class... Ts> \
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
template<class Tag, class T, class... Ts, class=typename std::enable_if<(std::is_class<T>::value)>::type> \
constexpr T ref alias_value(alias_inherit<T, Tag> ref a, Ts&&...) \
{ \
    return move(a); \
}
FIT_UNARY_PERFECT_FOREACH(FIT_DETAIL_ALIAS_INHERIT_GET_VALUE)

template<class T, class Tag>
struct alias_tag<alias_inherit<T, Tag>>
{ typedef Tag type; };

namespace detail {

template<class T, class Tag>
struct alias_static_storage
{
#ifdef _MSC_VER
    // Since we disable the error for 4579 on MSVC, which leaves the static
    // member unitialized at runtime, it is, therefore, only safe to use this
    // class on types that are empty with constructors that have no possible
    // side effects.
    static_assert(std::is_empty<T>::value && 
        std::is_literal_type<T>::value && 
        std::is_default_constructible<T>::value, "In-class initialization is not yet implemented on MSVC");
#endif
    static constexpr T value = T();
};

template<class T, class Tag>
constexpr T alias_static_storage<T, Tag>::value;

}

template<class T, class Tag=void>
struct alias_static
{
    template<class... Ts, FIT_ENABLE_IF_CONSTRUCTIBLE(T, Ts...)>
    constexpr alias_static(Ts&&...)
    {}
};

template<class Tag, class T, class... Ts>
constexpr const T& alias_value(const alias_static<T, Tag>&, Ts&&...)
{
    return detail::alias_static_storage<T, Tag>::value;
}

template<class T, class Tag>
struct alias_tag<alias_static<T, Tag>>
{ typedef Tag type; };

} // namespace fit

#endif
