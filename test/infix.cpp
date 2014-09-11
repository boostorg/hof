#include <fit/infix.h>
#include "test.h"

struct sum_f
{
    template<class T, class U>
    constexpr T operator()(T x, U y) const
    {
        return x+y;
    }
};

const constexpr fit::infix_adaptor<sum_f> sum = {};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == (1 <sum> 2));
    static_assert(3 == (1 <sum> 2), "Constexpr infix failed");
}
