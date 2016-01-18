#include <fit/args.hpp>
#include <type_traits>
#include "test.hpp"

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::args<3>(1,2,3,4,5) == 3);
    FIT_TEST_CHECK( fit::args<3>(1,2,3,4,5) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::args(std::integral_constant<int, 3>())(1,2,3,4,5) == 3);
    FIT_TEST_CHECK( fit::args(std::integral_constant<int, 3>())(1,2,3,4,5) == 3 );
}