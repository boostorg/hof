#include <fit/reverse_compress.h>
#include "test.h"

struct max_f
{
    template<class T, class U>
    constexpr T operator()(T x, U y) const
    {
        return x > y ? x : y;
    }
};

struct sum_f
{
    template<class T, class U>
    constexpr T operator()(T x, U y) const
    {
        return x + y;
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::reverse_compress(max_f(), 0)(2, 3, 4, 5) == 5);
    FIT_TEST_CHECK(fit::reverse_compress(max_f(), 0)(5, 4, 3, 2) == 5);
    FIT_TEST_CHECK(fit::reverse_compress(max_f(), 0)(2, 3, 5, 4) == 5);

    FIT_STATIC_TEST_CHECK(fit::reverse_compress(max_f(), 0)(2, 3, 4, 5) == 5);
    FIT_STATIC_TEST_CHECK(fit::reverse_compress(max_f(), 0)(5, 4, 3, 2) == 5);
    FIT_STATIC_TEST_CHECK(fit::reverse_compress(max_f(), 0)(2, 3, 5, 4) == 5);
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::reverse_compress(max_f(), 0)() == 0);
    FIT_TEST_CHECK(fit::reverse_compress(max_f(), 0)(5) == 5);

    FIT_STATIC_TEST_CHECK(fit::reverse_compress(max_f(), 0)() == 0);
    FIT_STATIC_TEST_CHECK(fit::reverse_compress(max_f(), 0)(5) == 5);
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::reverse_compress(max_f())(5) == 5);

    FIT_STATIC_TEST_CHECK(fit::reverse_compress(max_f())(5) == 5);
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::reverse_compress(max_f())(2, 3, 4, 5) == 5);
    FIT_TEST_CHECK(fit::reverse_compress(max_f())(5, 4, 3, 2) == 5);
    FIT_TEST_CHECK(fit::reverse_compress(max_f())(2, 3, 5, 4) == 5);

    FIT_STATIC_TEST_CHECK(fit::reverse_compress(max_f())(2, 3, 4, 5) == 5);
    FIT_STATIC_TEST_CHECK(fit::reverse_compress(max_f())(5, 4, 3, 2) == 5);
    FIT_STATIC_TEST_CHECK(fit::reverse_compress(max_f())(2, 3, 5, 4) == 5);
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::reverse_compress(sum_f(), std::string())("hello", "-", "world") == "world-hello");
}
