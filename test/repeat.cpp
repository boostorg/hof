#include <fit/repeat.hpp>
#include <limits>
#include "test.hpp"

// TODO: Add tests for multiple parameters

struct increment
{
    template<class T>
    constexpr T operator()(T x) const
    {
        return x + 1;
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::repeat(std::integral_constant<int, 5>())(increment())(1) == 6);
    FIT_STATIC_TEST_CHECK(fit::repeat(std::integral_constant<int, 5>())(increment())(1) == 6);
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::repeat(5)(increment())(1) == 6);
    FIT_STATIC_TEST_CHECK(fit::repeat(5)(increment())(1) == 6);
}

FIT_TEST_CASE()
{
    int i = 5;
    FIT_TEST_CHECK(fit::repeat(i)(increment())(1) == 6);
}

FIT_TEST_CASE()
{
    static const int i = 5;
    FIT_TEST_CHECK(fit::repeat(i)(increment())(1) == 6);
    FIT_STATIC_TEST_CHECK(fit::repeat(i)(increment())(1) == 6);
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::repeat(std::numeric_limits<int>::max())(increment())(0) == std::numeric_limits<int>::max());
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::repeat(FIT_RECURSIVE_CONSTEXPR_DEPTH+4)(increment())(0) == FIT_RECURSIVE_CONSTEXPR_DEPTH+4);
#if FIT_HAS_RELAXED_CONSTEXPR
    FIT_STATIC_TEST_CHECK(fit::repeat(FIT_RECURSIVE_CONSTEXPR_DEPTH+4)(increment())(0) == FIT_RECURSIVE_CONSTEXPR_DEPTH+4);
#endif
}
