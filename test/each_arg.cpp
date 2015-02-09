#include "test.h"
#include <fit/each_arg.h>

FIT_TEST_CASE()
{
    std::string s;
    auto f = [&](const std::string& x)
    {
        s += x;
    };
    fit::each_arg(f, "hello", "-", "world");
    FIT_TEST_CHECK(s == "hello-world");
    fit::each_arg(f);
    FIT_TEST_CHECK(s == "hello-world");
}
