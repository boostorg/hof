#include "test.h"
#include <fit/capture.h>

FIT_TEST_CASE()
{    
    FIT_STATIC_TEST_CHECK(fit::capture(1, 2)(binary_class())() == 3);
    FIT_TEST_CHECK(fit::capture(1, 2)(binary_class())() == 3);

    FIT_STATIC_TEST_CHECK(fit::capture(1)(binary_class())(2) == 3);
    FIT_TEST_CHECK(fit::capture(1)(binary_class())(2) == 3);

    // FIT_STATIC_TEST_CHECK(fit::capture_forward(1, 2)(binary_class())() == 3);
    FIT_TEST_CHECK(fit::capture_forward(1, 2)(binary_class())() == 3);

    FIT_STATIC_TEST_CHECK(fit::capture_forward(1)(binary_class())(2) == 3);
    FIT_TEST_CHECK(fit::capture_forward(1)(binary_class())(2) == 3);

    FIT_STATIC_TEST_CHECK(fit::capture_decay(1, 2)(binary_class())() == 3);
    FIT_TEST_CHECK(fit::capture_decay(1, 2)(binary_class())() == 3);

    FIT_STATIC_TEST_CHECK(fit::capture_decay(1)(binary_class())(2) == 3);
    FIT_TEST_CHECK(fit::capture_decay(1)(binary_class())(2) == 3);
}