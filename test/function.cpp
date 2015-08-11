#include <fit/function.h>
#include <fit/partial.h>
#include <fit/infix.h>
#include <memory>
#include "test.h"

namespace test_constexpr {

struct sum_f
{
    template<class T, class U>
    constexpr T operator()(T x, U y) const
    {
        return x+y;
    }
};

FIT_STATIC_FUNCTION(sum_init) = sum_f();

FIT_TEST_CASE()
{
// TODO: Should be empty on MSVC as well
#ifndef _MSC_VER
    STATIC_ASSERT_EMPTY(sum_init);
#endif
    FIT_TEST_CHECK(3 == sum_init(1, 2));

    FIT_STATIC_TEST_CHECK(3 == sum_init(1, 2));
}

FIT_STATIC_FUNCTION(sum_partial) = fit::partial(sum_f());
FIT_TEST_CASE()
{
#ifndef _MSC_VER
    STATIC_ASSERT_EMPTY(sum_partial);
#endif
    FIT_TEST_CHECK(3 == sum_partial(1, 2));
    FIT_TEST_CHECK(3 == sum_partial(1)(2));

    FIT_STATIC_TEST_CHECK(3 == sum_partial(1, 2));
    FIT_STATIC_TEST_CHECK(3 == sum_partial(1)(2));
}

}

namespace test_static {

struct sum_f
{
    template<class T, class U>
    constexpr T operator()(T x, U y) const
    {
        return x+y;
    }
};

struct add_one_f
{
    template<class T>
    constexpr T operator()(T x) const
    {
        return x+1;
    }
};

FIT_STATIC_FUNCTION(sum_partial) = fit::partial(sum_f());

FIT_TEST_CASE()
{
#ifndef _MSC_VER
    STATIC_ASSERT_EMPTY(sum_partial);
#endif
    FIT_TEST_CHECK(3 == sum_partial(1, 2));
    FIT_TEST_CHECK(3 == sum_partial(1)(2));
}

FIT_STATIC_FUNCTION(add_one_pipable) = fit::pipable(add_one_f());

FIT_TEST_CASE()
{
// TODO: Make this work on msvc
#ifndef _MSC_VER
    STATIC_ASSERT_EMPTY(add_one_pipable);
#endif
    FIT_TEST_CHECK(3 == add_one_pipable(2));
    FIT_TEST_CHECK(3 == (2 | add_one_pipable));
}

FIT_STATIC_FUNCTION(sum_infix) = fit::infix(sum_f());

FIT_TEST_CASE()
{
// TODO: Make this work on msvc
#ifndef _MSC_VER
    STATIC_ASSERT_EMPTY(sum_infix);
#endif
    FIT_TEST_CHECK(3 == (1 <sum_infix> 2));
}

}
