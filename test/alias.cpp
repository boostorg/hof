#include <fit/alias.hpp>
#include "test.hpp"

struct foo
{
    int i;
    foo(int i_) : i(i_)
    {}
};

FIT_TEST_CASE()
{
    fit::alias<int> ai = 5;
    FIT_TEST_CHECK(fit::alias_value(ai) == 5);
    fit::alias_inherit<foo> af = foo{5};
    FIT_TEST_CHECK(fit::alias_value(af).i == 5);
}

