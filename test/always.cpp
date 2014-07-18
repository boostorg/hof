#include <fit/always.h>
#include "test.h"

FIT_TEST_CASE()
{
    static const int ten = 10;
    static_assert(fit::always(ten)(1,2,3,4,5) == 10, "static always test failed");
    FIT_TEST_CHECK( fit::always(ten)(1,2,3,4,5) == 10 );
}