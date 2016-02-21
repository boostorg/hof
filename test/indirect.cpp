#include <fit/indirect.hpp>
#include "test.hpp"

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::indirect(std::unique_ptr<binary_class>(new binary_class()))(1, 2));
    FIT_TEST_CHECK(3 == fit::reveal(fit::indirect(std::unique_ptr<binary_class>(new binary_class())))(1, 2));

    binary_class f;

    FIT_TEST_CHECK(3 == fit::indirect(&f)(1, 2));
    FIT_TEST_CHECK(3 == fit::reveal(fit::indirect(&f))(1, 2));
}
