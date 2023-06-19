/*=============================================================================
    Copyright (c) 2017 Paul Fultz II
    substitute.cpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <boost/hof/function.hpp>
#include <boost/hof/lambda.hpp>
#include <boost/hof/placeholders.hpp>
#include <boost/hof/substitute.hpp>
#include "test.hpp"

namespace substitute_test {
struct fx_x_add_n {
    int n;
    constexpr fx_x_add_n(int n) : n(n) {}
    template<class T>
    constexpr T operator()(T x) const noexcept
    {
        return x + n;
    }
};

struct fx_x_times_n {
    int n;
    constexpr fx_x_times_n(int n) : n(n) {}
    template<class T>
    constexpr T operator()(T x) const noexcept
    {
        return x * n;
    }
};

struct fx_n_sub_x {
    int n;
    constexpr fx_n_sub_x(int n) : n(n) {}
    template<class T>
    constexpr T operator()(T x) const noexcept
    {
        return n - x;
    }
};

template <class F>
struct fx_ptr {
    std::unique_ptr<F> f;
    fx_ptr(int n) : f(new F(n)) {}
    template <class T>
    T operator()(T x) const
    {
        return (*f)(x);
    }
};

template<class T>
struct make {
    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const noexcept
    {
        return T(xs...);
    }
};

template<class T>
struct make_ptr {
    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const
    {
        return fx_ptr<T>(xs...);
    }
};

#if BOOST_HOF_HAS_NOEXCEPT_DEDUCTION
BOOST_HOF_TEST_CASE()
{
    static_assert(noexcept(boost::hof::substitute(
        make<fx_x_add_n>(), make<fx_x_add_n>(), boost::hof::identity)(3)), "noexcept substitute");
}
#endif 

BOOST_HOF_TEST_CASE(){
    BOOST_HOF_TEST_CHECK(boost::hof::substitute(boost::hof::always)(3)() == 3);
    BOOST_HOF_TEST_CHECK(boost::hof::substitute(boost::hof::always, boost::hof::always)(3) == 3);
    BOOST_HOF_TEST_CHECK(boost::hof::substitute(boost::hof::always, boost::hof::always, boost::hof::always)(3) == 3);

    BOOST_HOF_STATIC_TEST_CHECK(boost::hof::substitute(boost::hof::always)(3)() == 3);
    BOOST_HOF_STATIC_TEST_CHECK(boost::hof::substitute(boost::hof::always, boost::hof::always)(3) == 3);
    BOOST_HOF_STATIC_TEST_CHECK(boost::hof::substitute(boost::hof::always, boost::hof::always, boost::hof::always)(3) == 3);
}

BOOST_HOF_TEST_CASE()
{
    int r = boost::hof::substitute(make<fx_x_add_n>(), make<fx_x_add_n>(), boost::hof::identity)(3);
    BOOST_HOF_TEST_CHECK(r == 9);
    BOOST_HOF_STATIC_TEST_CHECK(boost::hof::substitute(
        make<fx_x_add_n>(), make<fx_x_add_n>(), boost::hof::identity)(3) == 9);
}

BOOST_HOF_TEST_CASE()
{
    int r = boost::hof::substitute(make<fx_x_add_n>(), make<fx_n_sub_x>(), make<fx_x_times_n>(), boost::hof::identity)(3);
    BOOST_HOF_TEST_CHECK(r == -3);
    BOOST_HOF_STATIC_TEST_CHECK(boost::hof::substitute(
        make<fx_x_add_n>(), make<fx_n_sub_x>(), make<fx_x_times_n>(), boost::hof::identity)(3) == -3);
}

BOOST_HOF_TEST_CASE()
{
    constexpr auto f = boost::hof::substitute(make<fx_x_add_n>(), boost::hof::identity);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(f)>::value, "Substitute function not empty");
#endif
    static_assert(BOOST_HOF_IS_DEFAULT_CONSTRUCTIBLE(decltype(f)), "Substitute function not default constructible");
    int r = f(3);
    BOOST_HOF_TEST_CHECK(r == 6);
    BOOST_HOF_STATIC_TEST_CHECK(f(3) == 6);
}

#ifndef _MSC_VER
BOOST_HOF_TEST_CASE()
{
    constexpr auto f = boost::hof::substitute(make<fx_x_add_n>(), make<fx_n_sub_x>(), make<fx_x_times_n>(), boost::hof::identity);
    static_assert(std::is_empty<decltype(f)>::value, "Substitute function not empty");
    static_assert(BOOST_HOF_IS_DEFAULT_CONSTRUCTIBLE(decltype(f)), "Substitute function not default constructible");
    int r = f(3);
    BOOST_HOF_TEST_CHECK(r == -3);
    BOOST_HOF_STATIC_TEST_CHECK(f(3) == -3);
}
#endif

BOOST_HOF_TEST_CASE()
{
    STATIC_ASSERT_MOVE_ONLY(fx_ptr<fx_x_add_n>);
    STATIC_ASSERT_MOVE_ONLY(fx_ptr<fx_n_sub_x>);
    STATIC_ASSERT_MOVE_ONLY(fx_ptr<fx_x_times_n>);
    int r = boost::hof::substitute(
        make_ptr<fx_x_add_n>(), make_ptr<fx_n_sub_x>(), make_ptr<fx_x_times_n>(), boost::hof::identity)(3);
    BOOST_HOF_TEST_CHECK(r == -3);
}

BOOST_HOF_TEST_CASE()
{
    auto make_lambda_add = [](int n) { 
        return [n](int x) { return x + n; };
    };
    auto make_lambda_times = [](int n) { 
        return [n](int x) { return x * n; };
    };
    {
        const auto f = boost::hof::substitute(
            make_lambda_add,
            make_lambda_times,
            [](int i) { return i+1; }
        );
#ifndef _MSC_VER
        static_assert(std::is_empty<decltype(f)>::value, "Substitute function not empty");
#endif
        int r = f(3);
        BOOST_HOF_TEST_CHECK(r == 15);
    }
    {
        BOOST_HOF_STATIC_LAMBDA_FUNCTION(f_substitute_lambda) = boost::hof::substitute(
            make_lambda_add, 
            make_lambda_times, 
            [](int i) { return i+1; }
        );
        int r = f_substitute_lambda(3);
        BOOST_HOF_TEST_CHECK(r == 15);
    }
}

BOOST_HOF_STATIC_FUNCTION(f_substitute_single_function) = boost::hof::substitute(fx_x_add_n(1));
BOOST_HOF_TEST_CASE()
{
    BOOST_HOF_TEST_CHECK(f_substitute_single_function(3) == 4);
    BOOST_HOF_STATIC_TEST_CHECK(f_substitute_single_function(3) == 4);
}

BOOST_HOF_STATIC_FUNCTION(f_substitute_function) = boost::hof::substitute(
    make<fx_x_add_n>(), make<fx_x_times_n>(), boost::hof::identity);
BOOST_HOF_TEST_CASE()
{
    BOOST_HOF_TEST_CHECK(f_substitute_function(3) == 12);
    BOOST_HOF_STATIC_TEST_CHECK(f_substitute_function(3) == 12);
}

BOOST_HOF_STATIC_FUNCTION(f_substitute_function_4) = boost::hof::substitute(
    make<fx_x_add_n>(), make<fx_n_sub_x>(), make<fx_x_times_n>(), boost::hof::identity
);
BOOST_HOF_TEST_CASE()
{
    BOOST_HOF_TEST_CHECK(f_substitute_function_4(3) == -3);
    BOOST_HOF_STATIC_TEST_CHECK(f_substitute_function_4(3) == -3);
}

BOOST_HOF_TEST_CASE()
{
    BOOST_HOF_TEST_CHECK(boost::hof::substitute(
        [](auto x) { return boost::hof::_1 * boost::hof::_1; },
        [](auto x) { return boost::hof::_1 + boost::hof::_1; },
        boost::hof::identity
        )(3) == 36);

    BOOST_HOF_STATIC_TEST_CHECK(boost::hof::substitute(
        [](auto x) { return boost::hof::_1 * boost::hof::_1; },
        [](auto x) { return boost::hof::_1 + boost::hof::_1; },
        boost::hof::identity
        )(3) == 36);
}
}  // namespace substitute_test
