#include <fit/identity.hpp>
#include <fit/is_callable.hpp>
#include "test.hpp"

FIT_TEST_CASE()
{
	FIT_STATIC_TEST_CHECK(fit::identity(10) == 10);
    FIT_TEST_CHECK(fit::identity(10) == 10);
}

FIT_TEST_CASE()
{
    int i = 3;
	FIT_TEST_CHECK(fit::identity(i) == 3);
    FIT_TEST_CHECK(&fit::identity(i) == &i);
    static_assert(std::is_lvalue_reference<decltype(fit::identity(i))>::value, "Not lvalue");
    static_assert(!std::is_lvalue_reference<decltype(fit::identity(3))>::value, "Not rvalue");
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(std::vector<int>(fit::identity({1, 2, 3})) == std::vector<int>{1, 2, 3});
}

FIT_TEST_CASE()
{
    static_assert(fit::is_callable<decltype(fit::identity), int>::value, "Identiy callable");
    static_assert(!fit::is_callable<decltype(fit::identity), int, int>::value, "Identiy not callable");
    static_assert(!fit::is_callable<decltype(fit::identity)>::value, "Identiy not callable");
}
