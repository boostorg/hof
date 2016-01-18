#include <fit/static.hpp>
#include "test.hpp"

// TODO: Test infix

static constexpr fit::static_<binary_class> binary_static = {};

static constexpr fit::static_<void_class> void_static = {};

static constexpr fit::static_<mono_class> mono_static = {};


FIT_TEST_CASE()
{
    void_static(1);
    FIT_TEST_CHECK(3 == binary_static(1, 2));
    FIT_TEST_CHECK(3 == mono_static(2));
}
