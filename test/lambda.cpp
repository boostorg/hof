#include <fit/lambda.h>
#include <fit/conditional.h>
#include <fit/partial.h>
#include <fit/infix.h>
#include <fit/pipable.h>
#include <memory>
#include "test.h"

static constexpr auto add_one = FIT_STATIC_LAMBDA(int x)
{
    return x + 1;
};

template<class F>
struct wrapper : F
{
    FIT_INHERIT_CONSTRUCTOR(wrapper, F)
};

template<class T>
constexpr wrapper<T> wrap(T x)
{
    return x;
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == add_one(2));
}

FIT_TEST_CASE()
{
    constexpr auto add_one_again = add_one;
    FIT_TEST_CHECK(3 == add_one_again(2));
}

FIT_TEST_CASE()
{
    constexpr auto add_one_again = wrap(add_one);
    FIT_TEST_CHECK(3 == add_one_again(2));
}

namespace test_static {

FIT_STATIC_LAMBDA_FUNCTION(sum_partial) = fit::partial([](int x, int y)
{
    return x + y;
});

FIT_TEST_CASE()
{
    STATIC_ASSERT_EMPTY(sum_partial);
    FIT_TEST_CHECK(3 == sum_partial(1, 2));
    FIT_TEST_CHECK(3 == sum_partial(1)(2));
}

FIT_STATIC_LAMBDA_FUNCTION(add_one_pipable) = fit::pipable([](int x)
{
    return x + 1;
});

FIT_TEST_CASE()
{
    STATIC_ASSERT_EMPTY(add_one_pipable);
    FIT_TEST_CHECK(3 == add_one_pipable(2));
    FIT_TEST_CHECK(3 == (2 | add_one_pipable));
}

FIT_STATIC_LAMBDA_FUNCTION(sum_infix) = fit::infix([](int x, int y)
{
    return x + y;
});

FIT_TEST_CASE()
{
    STATIC_ASSERT_EMPTY(sum_infix);
    FIT_TEST_CHECK(3 == (1 <sum_infix> 2));
}

}

