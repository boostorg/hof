#include <fit/infix.hpp>
#include "test.hpp"

struct sum_f
{
    template<class T, class U>
    constexpr T operator()(T x, U y) const
    {
        return x+y;
    }
};

static constexpr fit::infix_adaptor<sum_f> sum = {};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == (1 <sum> 2));
    FIT_STATIC_TEST_CHECK(3 == (1 <sum> 2));
}
