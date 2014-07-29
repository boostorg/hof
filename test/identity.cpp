#include <fit/identity.h>
#include "test.h"

FIT_TEST_CASE()
{
	static_assert(fit::identity(10) == 10, "Constexpr failed");
	FIT_TEST_CHECK(fit::identity(10) == 10);
}