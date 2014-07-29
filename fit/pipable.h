/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    pipable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_PIPABLE_H
#define FIT_GUARD_FUNCTION_PIPABLE_H

/// pipable
/// =======
/// 
/// Description
/// -----------
/// 
/// The `pipable` function adaptor provides an extension method. The first
/// parameter of the function can be piped into the function using the `|`
/// operator. This can be especially convient when there are a lot of nested
/// function calls. Functions that are made pipable can still be called the
/// traditional way without piping in the first parameter.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     pipable_adaptor<F> pipable(F f);
/// 
/// Example
/// -------
/// 
///     struct sum
///     {
///         template<class T>
///         T sum(T x, T y) const
///         {
///             return x+y;
///         }
///     };
/// 
///     assert(3 == 1 | pipable(sum())(2));
///     assert(3 == pipable(sum())(1, 2));
/// 

#include <fit/conditional.h>
#include <fit/static.h>
#include <fit/variadic.h>
#include <fit/invoke.h>

namespace fit { 
 
template<class F>
struct pipable_adaptor;

// A possible implementation using generic lambdas
#if 0 // FIT_NO_GENERIC_LAMBDA

template<class F>
struct pipe_closure : F
{
    template<class X>
    pipe_closure(X&& f) : F(std::forward<X>(f))
    {}

    const F& base_function() const
    {
        return *this;
    }
};

template<class A, class F>
decltype(auto) operator|(A&& a, const pipe_closure<F>& p)
{
    return p(std::forward<A>(a));
}

template<class F>
pipe_closure<F> make_pipe_closure(F&& x)
{
    return pipe_closure<F>(std::forward<F>(x));
}

template<class Derived, class F>
struct pipe_closure_factory
{
    const F& get_function() const
    {
        return static_cast<const F&>(static_cast<const Derived&>(*this));
    }

    template<class... Ts>
    auto operator()(Ts&&... xs) const
    {
        return make_pipe_closure([ys = std::forward<Ts>(xs)...](auto&& x)
        {
            return this->get_function()(x, std::forward<Ts>(ys)...);
        });
    }
};


template<class F>
using pipable_adaptor_base = pipe_closure_factory<F, pipe_closure_factory<pipable_adaptor<F>, F> >;
#else
namespace detail {

template<class F, class Sequence>
struct pipe_closure : F, Sequence
{
    
    template<class X, class S>
    constexpr pipe_closure(X&& f, S&& seq) : F(std::forward<X>(f)), Sequence(std::forward<S>(seq))
    {}

    constexpr const F& base_function() const
    {
        return *this;
    }

    constexpr const Sequence& get_sequence() const
    {
        return *this;
    }

    template<class A>
    constexpr decltype(auto) operator()(A&& a) const
    {
        return fit::invoke(this->base_function(), std::tuple_cat
        (
            std::forward_as_tuple(std::forward<A>(a)),
            this->get_sequence()
        ));
    }
};


template<class Derived, class F>
struct pipe_closure_factory
{
    constexpr const F& get_function() const
    {
        return static_cast<const F&>(static_cast<const Derived&>(*this));
    }

    template<class T>
    pipe_closure<F, T> 
    constexpr operator()(T&& t) const
    {
        typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type type;
        return pipe_closure<F, type>(this->get_function(), std::forward<T>(t)) ;
    }
};
    
template<class F>
using pipable_adaptor_base = conditional_adaptor<F, variadic_adaptor<detail::pipe_closure_factory<pipable_adaptor<F>, F> > >;
template<class A, class F, class Sequence>
constexpr decltype(auto) operator|(A&& a, const pipe_closure<F, Sequence>& p)
{
    return p(std::forward<A>(a));
}
#endif
}

template<class F>
struct pipable_adaptor 
: detail::pipable_adaptor_base<F>
{
    typedef detail::pipable_adaptor_base<F> base;

    constexpr pipable_adaptor()
    {}

    template<class X>
    constexpr pipable_adaptor(X&& x) : base(std::forward<X>(x), {})
    {}

    constexpr const F& base_function() const
    {
        return *this;
    }

    // MSVC Workaround
    // pipable_adaptor(const pipable_adaptor& rhs) : base(static_cast<const base&>(rhs))
    // {}
};


template<class F>
constexpr pipable_adaptor<F> pipable(F f)
{
    return pipable_adaptor<F>(f);
}

// Operators for static_ adaptor
template<class A, class F>
decltype(auto) operator|(A&& a, static_<F> f)
{
    return f.base_function().base_function()(std::forward<A>(a));
}
}

#endif
