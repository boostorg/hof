#include "test.h"
#include <fit/lift.h>
#include <tuple>

template<class T, class U>
constexpr T sum(T x, U y)
{
    return x + y;
}

FIT_LIFT_CLASS(max_f, std::max);
FIT_LIFT_CLASS(sum_f, sum);

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(max_f()(3, 4) == std::max(3, 4));

    FIT_TEST_CHECK(sum_f()(1, 2) == 3);
    FIT_STATIC_TEST_CHECK(sum_f()(1, 2) == 3);
}
