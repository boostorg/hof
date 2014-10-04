#include <fit/variadic.h>
#include "test.h"

typedef fit::variadic_adaptor<tuple_class> vard_class;

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == vard_class()(2));
    FIT_STATIC_TEST_CHECK(3 == vard_class()(2));
}