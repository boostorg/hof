#include <fit/static.h>
#include "test.h"

// TODO: Test infix

fit::static_<binary_class> binary_static = {};

fit::static_<void_class> void_static = {};

fit::static_<mono_class> mono_static = {};


FIT_TEST_CASE()
{
    void_static(1);
    FIT_TEST_CHECK(3 == binary_static(1, 2));
    FIT_TEST_CHECK(3 == mono_static(2));
}