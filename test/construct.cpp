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

BOOST_FIT_TEST_CASE()
{
    auto v = boost::fit::construct<std::vector<int>>()(5, 5);
    BOOST_FIT_TEST_CHECK(v.size() == 5);
    BOOST_FIT_TEST_CHECK(v == std::vector<int>{5, 5, 5, 5, 5});
}

BOOST_FIT_TEST_CASE()
{
    auto make = boost::fit::construct<std::vector<int>>().by(boost::fit::_1 * boost::fit::_1);
    auto v = make(3, 3);
    BOOST_FIT_TEST_CHECK(v.size() == 9);
    BOOST_FIT_TEST_CHECK(v == std::vector<int>{9, 9, 9, 9, 9, 9, 9, 9, 9});
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
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 7
    BOOST_FIT_STATIC_TEST_CHECK(std::make_tuple(1, 2, 3) == boost::fit::construct<std::tuple>()(1, 2, 3));
#endif
}

BOOST_FIT_TEST_CASE()
{
    auto t = boost::fit::construct<std::pair>()(1, 2);
    static_assert(std::is_same<std::pair<int, int>, decltype(t)>::value, "");
    BOOST_FIT_TEST_CHECK(t == std::make_pair(1, 2));
// GCC 4.7 doesn't have fully constexpr pair
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 7
    BOOST_FIT_STATIC_TEST_CHECK(std::make_pair(1, 2) == boost::fit::construct<std::pair>()(1, 2));
#endif
}

BOOST_FIT_TEST_CASE()
{
    auto make = boost::fit::construct<std::tuple>().by(boost::fit::_1 * boost::fit::_1);
    auto t = make(1, 2, 3);
    static_assert(std::is_same<std::tuple<int, int, int>, decltype(t)>::value, "");
    BOOST_FIT_TEST_CHECK(t == std::make_tuple(1, 4, 9));
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
    auto t = boost::fit::construct_meta<tuple_meta>()(1, 2, 3);
    static_assert(std::is_same<std::tuple<int, int, int>, decltype(t)>::value, "");
    BOOST_FIT_TEST_CHECK(t == std::make_tuple(1, 2, 3));
// GCC 4.7 doesn't have fully constexpr tuple
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 7
    BOOST_FIT_STATIC_TEST_CHECK(std::make_tuple(1, 2, 3) == boost::fit::construct_meta<tuple_meta>()(1, 2, 3));
#endif
}

BOOST_FIT_TEST_CASE()
{
    auto t = boost::fit::construct_meta<tuple_meta_class>()(1, 2, 3);
    static_assert(std::is_same<std::tuple<int, int, int>, decltype(t)>::value, "");
    BOOST_FIT_TEST_CHECK(t == std::make_tuple(1, 2, 3));
// GCC 4.7 doesn't have fully constexpr tuple
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 7
    BOOST_FIT_STATIC_TEST_CHECK(std::make_tuple(1, 2, 3) == boost::fit::construct_meta<tuple_meta_class>()(1, 2, 3));
#endif
}

BOOST_FIT_TEST_CASE()
{
    auto make = boost::fit::construct_meta<tuple_meta>().by(boost::fit::_1 * boost::fit::_1);
    auto t = make(1, 2, 3);
    static_assert(std::is_same<std::tuple<int, int, int>, decltype(t)>::value, "");
    BOOST_FIT_TEST_CHECK(t == std::make_tuple(1, 4, 9));
}

BOOST_FIT_TEST_CASE()
{
    auto make = boost::fit::construct_meta<tuple_meta_class>().by(boost::fit::_1 * boost::fit::_1);
    auto t = make(1, 2, 3);
    static_assert(std::is_same<std::tuple<int, int, int>, decltype(t)>::value, "");
    BOOST_FIT_TEST_CHECK(t == std::make_tuple(1, 4, 9));
}

