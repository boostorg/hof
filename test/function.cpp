#include <fit/function.h>
#include <fit/partial.h>
#include <memory>
#include "test.h"

FIT_STATIC_FUNCTION(sum) = fit::partial([](int x, int y)
{
    return x + y;
});

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == sum(1, 2));
    FIT_TEST_CHECK(3 == sum(1)(2));
}
