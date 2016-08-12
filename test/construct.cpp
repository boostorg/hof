#include <fit/construct.hpp>
#include "test.hpp"

#include <fit/conditional.hpp>
#include <fit/by.hpp>
#include <fit/placeholders.hpp>

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


FIT_TEST_CASE()
{
    auto v = fit::construct<std::vector<int>>()(5, 5);
    FIT_TEST_CHECK(v.size() == 5);
    FIT_TEST_CHECK(v == std::vector<int>{5, 5, 5, 5, 5});
}

FIT_TEST_CASE()
{
    auto v = fit::construct_basic<std::vector<int>>()(5, 5);
    FIT_TEST_CHECK(v.size() == 5);
    FIT_TEST_CHECK(v == std::vector<int>{5, 5, 5, 5, 5});
}

FIT_TEST_CASE()
{
    auto v = fit::construct_forward<std::vector<int>>()(5, 5);
    FIT_TEST_CHECK(v.size() == 5);
    FIT_TEST_CHECK(v == std::vector<int>{5, 5, 5, 5, 5});
}

FIT_TEST_CASE()
{
    auto x = fit::construct<implicit_default>()();
    FIT_TEST_CHECK(x.mem1 == 0);
    FIT_TEST_CHECK(x.mem2 == "");
}

FIT_TEST_CASE()
{
    auto x = fit::construct<user_default>()();
    FIT_TEST_CHECK(x.mem1 == 0);
    FIT_TEST_CHECK(x.mem2 == "");
}

FIT_TEST_CASE()
{
    auto x = fit::construct<user_construct>()(3);
    FIT_TEST_CHECK(x.mem1 == 0);
    FIT_TEST_CHECK(x.mem2 == "");
}

FIT_TEST_CASE()
{
    auto x = fit::construct<template_user_construct>()(3);
    FIT_TEST_CHECK(x.mem1 == 0);
    FIT_TEST_CHECK(x.mem2 == "");
}

FIT_TEST_CASE()
{
    auto x = fit::construct_forward<template_user_construct>()(3);
    FIT_TEST_CHECK(x.mem1 == 0);
    FIT_TEST_CHECK(x.mem2 == "");
}

FIT_TEST_CASE()
{
    auto x = fit::construct_basic<template_user_construct>()(3);
    FIT_TEST_CHECK(x.mem1 == 0);
    FIT_TEST_CHECK(x.mem2 == "");
}

FIT_TEST_CASE()
{
    auto v = fit::construct<std::vector<int>>()({5, 5, 5, 5, 5});
    FIT_TEST_CHECK(v.size() == 5);
    FIT_TEST_CHECK(v == std::vector<int>{5, 5, 5, 5, 5});
}

FIT_TEST_CASE()
{
    auto t = fit::construct<std::tuple>()(1, 2, 3);
    static_assert(std::is_same<std::tuple<int, int, int>, decltype(t)>::value, "");
    FIT_TEST_CHECK(t == std::make_tuple(1, 2, 3));
// GCC 4.7 doesn't have fully constexpr tuple
#if defined (__clang__) || !(defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8)
    FIT_STATIC_TEST_CHECK(std::make_tuple(1, 2, 3) == fit::construct<std::tuple>()(1, 2, 3));
#endif
}

FIT_TEST_CASE()
{
    auto t = fit::construct<std::pair>()(1, 2);
    static_assert(std::is_same<std::pair<int, int>, decltype(t)>::value, "");
    FIT_TEST_CHECK(t == std::make_pair(1, 2));
// GCC 4.7 doesn't have fully constexpr pair
#if defined (__clang__) || !(defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8)
    FIT_STATIC_TEST_CHECK(std::make_pair(1, 2) == fit::construct<std::pair>()(1, 2));
#endif
}

FIT_TEST_CASE()
{
    auto f = fit::conditional(fit::construct<std::pair>(), fit::identity);
    FIT_TEST_CHECK(f(1, 2) == std::make_pair(1, 2));
    FIT_TEST_CHECK(f(1) == 1);
}

FIT_TEST_CASE()
{
    auto x = fit::construct<ac>()(1);
    static_assert(std::is_same<ac<int>, decltype(x)>::value, "");
    FIT_TEST_CHECK(x.value == ac<int>(1).value);
    FIT_STATIC_TEST_CHECK(ac<int>(1).value == fit::construct<ac>()(1).value);
}

FIT_TEST_CASE()
{
    auto x = fit::construct_basic<ac>()(1);
    static_assert(std::is_same<ac<int>, decltype(x)>::value, "");
    FIT_TEST_CHECK(x.value == ac<int>(1).value);
    FIT_STATIC_TEST_CHECK(ac<int>(1).value == fit::construct<ac>()(1).value);
}

FIT_TEST_CASE()
{
    int i = 1;
    auto x = fit::construct_forward<ac>()(i);
    static_assert(std::is_same<ac<int&>, decltype(x)>::value, "");
    FIT_TEST_CHECK(&x.value == &i);
}

FIT_TEST_CASE()
{
    int i = 1;
    auto x = fit::construct_basic<ac>()(i);
    static_assert(std::is_same<ac<int&>, decltype(x)>::value, "");
    FIT_TEST_CHECK(&x.value == &i);
}

FIT_TEST_CASE()
{
    auto t = fit::construct_meta<tuple_meta>()(1, 2, 3);
    static_assert(std::is_same<std::tuple<int, int, int>, decltype(t)>::value, "");
    FIT_TEST_CHECK(t == std::make_tuple(1, 2, 3));
// GCC 4.7 doesn't have fully constexpr tuple
#if defined (__clang__) || !(defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8)
    FIT_STATIC_TEST_CHECK(std::make_tuple(1, 2, 3) == fit::construct_meta<tuple_meta>()(1, 2, 3));
#endif
}

FIT_TEST_CASE()
{
    auto t = fit::construct_meta<tuple_meta_class>()(1, 2, 3);
    static_assert(std::is_same<std::tuple<int, int, int>, decltype(t)>::value, "");
    FIT_TEST_CHECK(t == std::make_tuple(1, 2, 3));
// GCC 4.7 doesn't have fully constexpr tuple
#if defined (__clang__) || !(defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8)
    FIT_STATIC_TEST_CHECK(std::make_tuple(1, 2, 3) == fit::construct_meta<tuple_meta_class>()(1, 2, 3));
#endif
}

