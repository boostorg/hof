#include <fit/rotate.hpp>
#include <fit/placeholders.hpp>
#include "test.hpp"

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::rotate(fit::_ - fit::_)(2, 5));
    FIT_STATIC_TEST_CHECK(3 == fit::rotate(fit::_ - fit::_)(2, 5));
}
