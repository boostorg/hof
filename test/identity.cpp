#include <fit/identity.hpp>
#include "test.hpp"

FIT_TEST_CASE()
{
	FIT_STATIC_TEST_CHECK(fit::identity(10) == 10);
	FIT_TEST_CHECK(fit::identity(10) == 10);
}
