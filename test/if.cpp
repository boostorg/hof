#include <fit/if.h>
#include "test.h"

#include <fit/conditional.h>
#include <fit/placeholders.h>


struct is_5
{
    template<class T>
    constexpr bool operator()(T i) const
    {
        return i == 5;
    }
};

struct is_not_5
{
    template<class T>
    constexpr bool operator()(T i) const
    {
        return i != 5;
    }
};

template<class F>
struct test_int
{
    template<class T>
    constexpr bool operator()(T x) const
    {
        return fit::conditional(
            fit::if_(std::is_integral<T>())(F()),
            fit::always(true)
        )(x);
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(test_int<is_5>()(5));
    FIT_TEST_CHECK(test_int<is_5>()(5L));
    FIT_TEST_CHECK(test_int<is_5>()(5.0));
    FIT_TEST_CHECK(test_int<is_5>()(6.0));

    FIT_TEST_CHECK(test_int<is_not_5>()(6));
    FIT_TEST_CHECK(test_int<is_not_5>()(6L));
    FIT_TEST_CHECK(test_int<is_not_5>()(5.0));
    FIT_TEST_CHECK(test_int<is_not_5>()(6.0));

    FIT_STATIC_TEST_CHECK(test_int<is_5>()(5));
    FIT_STATIC_TEST_CHECK(test_int<is_5>()(5L));
    FIT_STATIC_TEST_CHECK(test_int<is_5>()(5.0));
    FIT_STATIC_TEST_CHECK(test_int<is_5>()(6.0));

    FIT_STATIC_TEST_CHECK(test_int<is_not_5>()(6));
    FIT_STATIC_TEST_CHECK(test_int<is_not_5>()(6L));
    FIT_STATIC_TEST_CHECK(test_int<is_not_5>()(5.0));
    FIT_STATIC_TEST_CHECK(test_int<is_not_5>()(6.0));
}

struct sum_f
{
    template<class T>
    constexpr int operator()(T x, T y) const
    {
        return fit::conditional(
            fit::if_(std::is_integral<T>())(fit::_ + fit::_),
            fit::always(0)
        )(x, y);
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(sum_f()(1, 2) == 3);
    FIT_TEST_CHECK(sum_f()(1.0, 2.0) == 0);
    FIT_TEST_CHECK(sum_f()("", "") == 0);

    FIT_STATIC_TEST_CHECK(sum_f()(1, 2) == 3);
    FIT_STATIC_TEST_CHECK(sum_f()("", "") == 0);
}


