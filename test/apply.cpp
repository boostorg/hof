#include <fit/apply.h>
#include "test.h"

FIT_TEST_CASE()
{    
    FIT_STATIC_TEST_CHECK(fit::apply(binary_class(), 1, 2) == 3);
    FIT_TEST_CHECK(fit::apply(binary_class(), 1, 2) == 3);
}
