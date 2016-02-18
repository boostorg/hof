#include <fit/alias.hpp>
#include "test.hpp"

struct foo
{};

FIT_TEST_CASE()
{
    fit::alias<int> ai = 5;
    fit::alias_inherit<foo> af = foo{};
}

