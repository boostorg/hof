#include <boost/fit/combine.hpp>
#include "test.hpp"

#include <boost/fit/construct.hpp>
#include <boost/fit/capture.hpp>
#include <utility>
#include <tuple>

template<class T, class U>
struct mini_pair
{
    T first;
    U second;

    template<class X, class Y>
    constexpr mini_pair(X&& x, Y&& y)
    : first(boost::fit::forward<X>(x)), second(boost::fit::forward<Y>(y))
    {}
};

template<class T1, class U1, class T2, class U2>
constexpr bool operator==(const mini_pair<T1, U1>& x, const mini_pair<T2, U2>& y)
{
    return x.first == y.first && x.second == y.second;
}

template<class T, class U>
constexpr mini_pair<T, U> make_mini_pair(T x, U y)
{
    return mini_pair<T, U>(x, y);
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(
        boost::fit::combine(
            boost::fit::construct<std::tuple>(),
            boost::fit::capture(1)(boost::fit::construct<std::pair>()),
            boost::fit::capture(2)(boost::fit::construct<std::pair>())
        )(2, 4) 
        == std::make_tuple(std::make_pair(1, 2), std::make_pair(2, 4)));
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(
        boost::fit::combine(
            boost::fit::construct<mini_pair>(),
            boost::fit::capture(1)(boost::fit::construct<mini_pair>()),
            boost::fit::capture(2)(boost::fit::construct<mini_pair>())
        )(2, 4) 
        == make_mini_pair(make_mini_pair(1, 2), make_mini_pair(2, 4)));

    BOOST_FIT_STATIC_TEST_CHECK(
        boost::fit::combine(
            boost::fit::construct<mini_pair>(),
            boost::fit::capture(1)(boost::fit::construct<mini_pair>()),
            boost::fit::capture(2)(boost::fit::construct<mini_pair>())
        )(2, 4) 
        == make_mini_pair(make_mini_pair(1, 2), make_mini_pair(2, 4)));
}




