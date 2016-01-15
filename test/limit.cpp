#include <fit/limit.h>
#include "test.h"

FIT_TEST_CASE()
{
    auto f = fit::limit(std::integral_constant<int, 2>())(binary_class());
    FIT_TEST_CHECK(f(1, 2) == 3);
    static_assert(fit::function_param_limit<decltype(f)>::value == 2, "Function limit is 2");
}

FIT_TEST_CASE()
{
    auto f = fit::limit_c<2>(binary_class());
    FIT_TEST_CHECK(f(1, 2) == 3);
    static_assert(fit::function_param_limit<decltype(f)>::value == 2, "Function limit is 2");
}
