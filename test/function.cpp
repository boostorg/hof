#include <fit/function.h>
#include <fit/partial.h>
#include <fit/infix.h>
#include <fit/pipable.h>
#include <memory>
#include "test.h"

FIT_STATIC_FUNCTION(sum_partial) = fit::partial([](int x, int y)
{
    return x + y;
});

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == sum_partial(1, 2));
    FIT_TEST_CHECK(3 == sum_partial(1)(2));
}

FIT_STATIC_FUNCTION(add_one_pipable) = fit::pipable([](int x)
{
    return x + 1;
});

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == add_one_pipable(2));
    FIT_TEST_CHECK(3 == (2 | add_one_pipable));
}

FIT_STATIC_FUNCTION(sum_infix) = fit::infix([](int x, int y)
{
    return x + y;
});

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == (1 <sum_infix> 2));
}
