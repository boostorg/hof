#include <fit/indirect.h>
#include "test.h"

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::indirect(std::unique_ptr<binary_class>(new binary_class()))(1, 2));
    FIT_TEST_CHECK(3 == fit::reveal(fit::indirect(std::unique_ptr<binary_class>(new binary_class())))(1, 2));
}
