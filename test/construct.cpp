#include <boost/fit/construct.hpp>
#include "test.hpp"

#include <boost/fit/conditional.hpp>
#include <boost/fit/by.hpp>
#include <boost/fit/placeholders.hpp>

#include <tuple>
#include <type_traits>
#include <vector>

template<class T>
struct ac
{
    T value;
    constexpr ac(T i) : value(i)
    {}
};

template<class... Ts>
struct tuple_meta
{
    typedef std::tuple<Ts...> type;
};

struct tuple_meta_class
{
    template<class... Ts>
    struct apply
    {
        typedef std::tuple<Ts...> type;
    };
};

struct implicit_default
{
    int mem1;
    std::string mem2;
};
 
struct user_default
{
    int mem1;
    std::string mem2;
    user_default() { }
};

struct user_construct
{
    int mem1;
    std::string mem2;
    user_construct(int) { }
};

template<class T>
struct template_user_construct
{
    int mem1;
    std::string mem2;
    template_user_construct(T) { }
};


BOOST_FIT_TEST_CASE()
{
    auto v = boost::fit::construct<std::vector<int>>()(5, 5);
    BOOST_FIT_TEST_CHECK(v.size() == 5);
    BOOST_FIT_TEST_CHECK(v == std::vector<int>{5, 5, 5, 5, 5});
}

BOOST_FIT_TEST_CASE()
{
    auto v = boost::fit::construct_basic<std::vector<int>>()(5, 5);
    BOOST_FIT_TEST_CHECK(v.size() == 5);
    BOOST_FIT_TEST_CHECK(v == std::vector<int>{5, 5, 5, 5, 5});
}

BOOST_FIT_TEST_CASE()
{
    auto v = boost::fit::construct_forward<std::vector<int>>()(5, 5);
    BOOST_FIT_TEST_CHECK(v.size() == 5);
    BOOST_FIT_TEST_CHECK(v == std::vector<int>{5, 5, 5, 5, 5});
}

BOOST_FIT_TEST_CASE()
{
    auto x = boost::fit::construct<implicit_default>()();
    BOOST_FIT_TEST_CHECK(x.mem1 == 0);
    BOOST_FIT_TEST_CHECK(x.mem2 == "");
}

BOOST_FIT_TEST_CASE()
{
    auto x = boost::fit::construct<user_default>()();
    BOOST_FIT_TEST_CHECK(x.mem1 == 0);
    BOOST_FIT_TEST_CHECK(x.mem2 == "");
}

BOOST_FIT_TEST_CASE()
{
    auto x = boost::fit::construct<user_construct>()(3);
    BOOST_FIT_TEST_CHECK(x.mem1 == 0);
    BOOST_FIT_TEST_CHECK(x.mem2 == "");
}

BOOST_FIT_TEST_CASE()
{
    auto x = boost::fit::construct<template_user_construct>()(3);
    BOOST_FIT_TEST_CHECK(x.mem1 == 0);
    BOOST_FIT_TEST_CHECK(x.mem2 == "");
}

BOOST_FIT_TEST_CASE()
{
    auto x = boost::fit::construct_forward<template_user_construct>()(3);
    BOOST_FIT_TEST_CHECK(x.mem1 == 0);
    BOOST_FIT_TEST_CHECK(x.mem2 == "");
}

BOOST_FIT_TEST_CASE()
{
    auto x = boost::fit::construct_basic<template_user_construct>()(3);
    BOOST_FIT_TEST_CHECK(x.mem1 == 0);
    BOOST_FIT_TEST_CHECK(x.mem2 == "");
}

BOOST_FIT_TEST_CASE()
{
    auto v = boost::fit::construct<std::vector<int>>()({5, 5, 5, 5, 5});
    BOOST_FIT_TEST_CHECK(v.size() == 5);
    BOOST_FIT_TEST_CHECK(v == std::vector<int>{5, 5, 5, 5, 5});
}

BOOST_FIT_TEST_CASE()
{
    auto t = boost::fit::construct<std::tuple>()(1, 2, 3);
    static_assert(std::is_same<std::tuple<int, int, int>, decltype(t)>::value, "");
    BOOST_FIT_TEST_CHECK(t == std::make_tuple(1, 2, 3));
// GCC 4.7 doesn't have fully constexpr tuple
#if defined (__clang__) || !(defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8)
    BOOST_FIT_STATIC_TEST_CHECK(std::make_tuple(1, 2, 3) == boost::fit::construct<std::tuple>()(1, 2, 3));
#endif
}

BOOST_FIT_TEST_CASE()
{
    auto t = boost::fit::construct<std::pair>()(1, 2);
    static_assert(std::is_same<std::pair<int, int>, decltype(t)>::value, "");
    BOOST_FIT_TEST_CHECK(t == std::make_pair(1, 2));
// GCC 4.7 doesn't have fully constexpr pair
#if defined (__clang__) || !(defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8)
    BOOST_FIT_STATIC_TEST_CHECK(std::make_pair(1, 2) == boost::fit::construct<std::pair>()(1, 2));
#endif
}

BOOST_FIT_TEST_CASE()
{
    auto f = boost::fit::conditional(boost::fit::construct<std::pair>(), boost::fit::identity);
    BOOST_FIT_TEST_CHECK(f(1, 2) == std::make_pair(1, 2));
    BOOST_FIT_TEST_CHECK(f(1) == 1);
}

BOOST_FIT_TEST_CASE()
{
    auto x = boost::fit::construct<ac>()(1);
    static_assert(std::is_same<ac<int>, decltype(x)>::value, "");
    BOOST_FIT_TEST_CHECK(x.value == ac<int>(1).value);
    BOOST_FIT_STATIC_TEST_CHECK(ac<int>(1).value == boost::fit::construct<ac>()(1).value);
}

BOOST_FIT_TEST_CASE()
{
    auto x = boost::fit::construct_basic<ac>()(1);
    static_assert(std::is_same<ac<int>, decltype(x)>::value, "");
    BOOST_FIT_TEST_CHECK(x.value == ac<int>(1).value);
    BOOST_FIT_STATIC_TEST_CHECK(ac<int>(1).value == boost::fit::construct<ac>()(1).value);
}

BOOST_FIT_TEST_CASE()
{
    int i = 1;
    auto x = boost::fit::construct_forward<ac>()(i);
    static_assert(std::is_same<ac<int&>, decltype(x)>::value, "");
    BOOST_FIT_TEST_CHECK(&x.value == &i);
}

BOOST_FIT_TEST_CASE()
{
    int i = 1;
    auto x = boost::fit::construct_basic<ac>()(i);
    static_assert(std::is_same<ac<int&>, decltype(x)>::value, "");
    BOOST_FIT_TEST_CHECK(&x.value == &i);
}

BOOST_FIT_TEST_CASE()
{
    auto t = boost::fit::construct_meta<tuple_meta>()(1, 2, 3);
    static_assert(std::is_same<std::tuple<int, int, int>, decltype(t)>::value, "");
    BOOST_FIT_TEST_CHECK(t == std::make_tuple(1, 2, 3));
// GCC 4.7 doesn't have fully constexpr tuple
#if defined (__clang__) || !(defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8)
    BOOST_FIT_STATIC_TEST_CHECK(std::make_tuple(1, 2, 3) == boost::fit::construct_meta<tuple_meta>()(1, 2, 3));
#endif
}

BOOST_FIT_TEST_CASE()
{
    auto t = boost::fit::construct_meta<tuple_meta_class>()(1, 2, 3);
    static_assert(std::is_same<std::tuple<int, int, int>, decltype(t)>::value, "");
    BOOST_FIT_TEST_CHECK(t == std::make_tuple(1, 2, 3));
// GCC 4.7 doesn't have fully constexpr tuple
#if defined (__clang__) || !(defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8)
    BOOST_FIT_STATIC_TEST_CHECK(std::make_tuple(1, 2, 3) == boost::fit::construct_meta<tuple_meta_class>()(1, 2, 3));
#endif
}

