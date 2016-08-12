#include <fit/combine.hpp>
#include "test.hpp"

#include <fit/construct.hpp>
#include <fit/capture.hpp>
#include <utility>
#include <tuple>

template<class T, class U>
struct mini_pair
{
    T first;
    U second;

    template<class X, class Y>
    constexpr mini_pair(X&& x, Y&& y)
    : first(fit::forward<X>(x)), second(fit::forward<Y>(y))
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

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(
        fit::combine(
            fit::construct<std::tuple>(),
            fit::capture_basic(1)(fit::construct<std::pair>()),
            fit::capture_basic(2)(fit::construct<std::pair>())
        )(2, 4) 
        == std::make_tuple(std::make_pair(1, 2), std::make_pair(2, 4)));
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(
        fit::combine(
            fit::construct<mini_pair>(),
            fit::capture_basic(1)(fit::construct<mini_pair>()),
            fit::capture_basic(2)(fit::construct<mini_pair>())
        )(2, 4) 
        == make_mini_pair(make_mini_pair(1, 2), make_mini_pair(2, 4)));

    FIT_STATIC_TEST_CHECK(
        fit::combine(
            fit::construct<mini_pair>(),
            fit::capture_basic(1)(fit::construct<mini_pair>()),
            fit::capture_basic(2)(fit::construct<mini_pair>())
        )(2, 4) 
        == make_mini_pair(make_mini_pair(1, 2), make_mini_pair(2, 4)));
}




