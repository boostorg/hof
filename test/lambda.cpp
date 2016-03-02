#include <boost/fit/lambda.hpp>
#include <boost/fit/conditional.hpp>
#include <boost/fit/partial.hpp>
#include <boost/fit/infix.hpp>
#include <boost/fit/pipable.hpp>
#include <memory>
#include "test.hpp"

#if BOOST_FIT_HAS_STATIC_LAMBDA
static constexpr auto add_one = BOOST_FIT_STATIC_LAMBDA(int x)
{
    return x + 1;
};

template<class F>
struct wrapper : F
{
    BOOST_FIT_INHERIT_CONSTRUCTOR(wrapper, F)
};

template<class T>
constexpr wrapper<T> wrap(T x)
{
    return x;
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == add_one(2));
}

BOOST_FIT_TEST_CASE()
{
    constexpr auto add_one_again = add_one;
    BOOST_FIT_TEST_CHECK(3 == add_one_again(2));
}

BOOST_FIT_TEST_CASE()
{
    constexpr auto add_one_again = wrap(add_one);
    BOOST_FIT_TEST_CHECK(3 == add_one_again(2));
}

namespace test_static {

BOOST_FIT_STATIC_LAMBDA_FUNCTION(add_one) = [](int x)
{
    return x + 1;
};

BOOST_FIT_STATIC_LAMBDA_FUNCTION(sum_partial) = boost::fit::partial([](int x, int y)
{
    return x + y;
});

BOOST_FIT_TEST_CASE()
{
#ifndef _MSC_VER
    STATIC_ASSERT_EMPTY(sum_partial);
#endif
    BOOST_FIT_TEST_CHECK(3 == sum_partial(1, 2));
    BOOST_FIT_TEST_CHECK(3 == sum_partial(1)(2));
}

BOOST_FIT_STATIC_LAMBDA_FUNCTION(add_one_pipable) = boost::fit::pipable([](int x)
{
    return x + 1;
});

BOOST_FIT_TEST_CASE()
{
#ifndef _MSC_VER
    STATIC_ASSERT_EMPTY(add_one_pipable);
#endif
    BOOST_FIT_TEST_CHECK(3 == add_one_pipable(2));
    BOOST_FIT_TEST_CHECK(3 == (2 | add_one_pipable));
}

BOOST_FIT_STATIC_LAMBDA_FUNCTION(sum_infix) = boost::fit::infix([](int x, int y)
{
    return x + y;
});

BOOST_FIT_TEST_CASE()
{
#ifndef _MSC_VER
    STATIC_ASSERT_EMPTY(sum_infix);
#endif
    BOOST_FIT_TEST_CHECK(3 == (1 <sum_infix> 2));
}

}
#endif
