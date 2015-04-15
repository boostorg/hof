#include <fit/flip.h>
#include <fit/placeholders.h>
#include "test.h"

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::flip(fit::_ - fit::_)(2, 5));
    FIT_STATIC_TEST_CHECK(3 == fit::flip(fit::_ - fit::_)(2, 5));
}