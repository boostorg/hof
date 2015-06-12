#include <fit/construct.h>
#include "test.h"

#include <tuple>
#include <type_traits>
#include <vector>

FIT_TEST_CASE()
{
    auto v = fit::construct<std::vector<int>>()(5, 5);
    FIT_TEST_CHECK(v.size() == 5);
    FIT_TEST_CHECK(v == std::vector<int>{5, 5, 5, 5, 5});
}

FIT_TEST_CASE()
{
    auto t = fit::construct<std::tuple>()(1, 2, 3);
    static_assert(std::is_same<std::tuple<int, int, int>, decltype(t)>::value, "");
    FIT_TEST_CHECK(t == std::make_tuple(1, 2, 3));
    FIT_STATIC_TEST_CHECK(std::make_tuple(1, 2, 3) == fit::construct<std::tuple>()(1, 2, 3));
}
