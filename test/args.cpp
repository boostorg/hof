#include <fit/args.h>
#include "test.h"

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::args<3>(1,2,3,4,5) == 3);
    FIT_TEST_CHECK( fit::args<3>(1,2,3,4,5) == 3 );
}