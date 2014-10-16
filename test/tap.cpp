#include <fit/tap.h>
#include "test.h"

struct sum_f
{
    template<class T, class U>
    constexpr T operator()(T x, U y) const
    {
        return x+y;
    }
};

static constexpr fit::pipable_adaptor<sum_f> sum = {};
// TODO: Test constexpr
FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == (1 | sum(2)));
    FIT_TEST_CHECK(5 == (1 | sum(2) | fit::tap([](int i) { FIT_TEST_CHECK(3 == i); }) | sum(2)));
}
