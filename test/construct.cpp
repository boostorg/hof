#include <fit/construct.h>
#include "test.h"

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

FIT_TEST_CASE()
{
    auto v = fit::construct<std::vector<int>>()(5, 5);
    FIT_TEST_CHECK(v.size() == 5);
    FIT_TEST_CHECK(v == std::vector<int>{5, 5, 5, 5, 5});
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
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 7
    FIT_STATIC_TEST_CHECK(std::make_tuple(1, 2, 3) == fit::construct<std::tuple>()(1, 2, 3));
#endif
}

FIT_TEST_CASE()
{
    auto x = fit::construct<ac>()(1);
    static_assert(std::is_same<ac<int>, decltype(x)>::value, "");
    FIT_TEST_CHECK(x.value == ac<int>(1).value);
    FIT_STATIC_TEST_CHECK(ac<int>(1).value == fit::construct<ac>()(1).value);
}
