#include <fit/repeat.hpp>
#include "test.hpp"


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
