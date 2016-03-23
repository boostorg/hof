#include <fit/arg.hpp>
#include <fit/is_callable.hpp>
#include <type_traits>
#include "test.hpp"

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::arg_c<3>(1,2,3,4,5) == 3);
    FIT_TEST_CHECK( fit::arg_c<3>(1,2,3,4,5) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::arg(std::integral_constant<int, 3>())(1,2,3,4,5) == 3);
    FIT_TEST_CHECK( fit::arg(std::integral_constant<int, 3>())(1,2,3,4,5) == 3 );
}

FIT_TEST_CASE()
{
    auto at_3 = fit::arg(std::integral_constant<int, 3>());
    static_assert(fit::is_callable<decltype(at_3), int, int, int>::value, "Not SFINAE-friendly");
    static_assert(!fit::is_callable<decltype(at_3), int, int>::value, "Not SFINAE-friendly");
    static_assert(!fit::is_callable<decltype(at_3), int>::value, "Not SFINAE-friendly");
}
