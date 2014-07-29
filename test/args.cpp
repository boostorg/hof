#include <fit/args.h>
#include "test.h"

FIT_TEST_CASE()
{
    static_assert(fit::args<3>(1,2,3,4,5) == 3, "static args test failed");
    FIT_TEST_CHECK( fit::args<3>(1,2,3,4,5) == 3 );
}