#include <fit/limit.hpp>
#include <fit/is_callable.hpp>
#include "test.hpp"

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

FIT_TEST_CASE()
{
    auto f = fit::limit_c<2>(fit::always(3));
    FIT_TEST_CHECK(f(1, 2) == 3);
    FIT_TEST_CHECK(f(1) == 3);
    FIT_TEST_CHECK(f() == 3);
    static_assert(fit::function_param_limit<decltype(f)>::value == 2, "Function limit is 2");
    static_assert(fit::is_callable<decltype(f), int>::value, "Callable");
    static_assert(fit::is_callable<decltype(f), int, int>::value, "Callable");
    static_assert(!fit::is_callable<decltype(f), int, int, int>::value, "Not Callable");
}

FIT_TEST_CASE()
{
    static_assert(!fit::is_callable<decltype(fit::limit), int>::value, "Not integral constant");
}

