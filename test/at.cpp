#include <fit/at.h>
#include <type_traits>
#include "test.h"

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::at(std::integral_constant<int, 3>())(0,1,2,3,4,5) == 3);
    FIT_TEST_CHECK( fit::at(std::integral_constant<int, 3>())(0,1,2,3,4,5) == 3 );
}
