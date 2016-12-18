#include "test.hpp"
#include <fit/lift.hpp>
#include <fit/function.hpp>
#include <fit/detail/move.hpp>
#include <tuple>
#include <algorithm>

template<class T, class U>
constexpr T sum(T x, U y) FIT_RETURNS_DEDUCE_NOEXCEPT(x+y)
{
    return x + y;
}

FIT_LIFT_CLASS(max_f, std::max);
FIT_LIFT_CLASS(sum_f, sum);

#if FIT_HAS_NOEXCEPT_DEDUCTION
FIT_TEST_CASE()
{
    static_assert(noexcept(sum_f()(1, 2)), "noexcept lift");
    static_assert(!noexcept(sum_f()(std::string(), std::string())), "noexcept lift");
}
#endif

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
