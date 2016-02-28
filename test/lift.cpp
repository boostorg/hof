#include "test.hpp"
#include <fit/lift.hpp>
#include <fit/function.hpp>
#include <fit/detail/move.hpp>
#include <tuple>
#include <algorithm>

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

#if FIT_HAS_GENERIC_LAMBDA
FIT_TEST_CASE()
{
    auto my_max = FIT_LIFT(std::max);
    FIT_TEST_CHECK(my_max(3, 4) == std::max(3, 4));
    
    FIT_TEST_CHECK(FIT_LIFT(std::max)(3, 4) == std::max(3, 4));
    FIT_TEST_CHECK(FIT_LIFT(sum)(1, 2) == 3);
}

FIT_STATIC_FUNCTION(psum) = FIT_LIFT(sum);
FIT_STATIC_FUNCTION(pmax) = FIT_LIFT(std::max);

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(pmax(3, 4) == std::max(3, 4));

    FIT_TEST_CHECK(psum(1, 2) == 3);
}
#endif
