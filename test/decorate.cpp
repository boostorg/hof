#include <fit/decorate.hpp>
#include "test.hpp"

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::decorate(fit::always(1))(fit::always(1))(fit::always(1))(5) == 1);
}
