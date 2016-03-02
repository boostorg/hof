#include <boost/fit/function.hpp>
#include <boost/fit/partial.hpp>
#include <boost/fit/infix.hpp>
#include <memory>
#include "test.hpp"

namespace test_constexpr {

struct sum_f
{
    template<class T, class U>
    constexpr T operator()(T x, U y) const
    {
        return x+y;
    }
};

BOOST_FIT_STATIC_FUNCTION(sum_init) = sum_f();

BOOST_FIT_TEST_CASE()
{
// TODO: Should be empty on MSVC as well
#ifndef _MSC_VER
    STATIC_ASSERT_EMPTY(sum_init);
#endif
    BOOST_FIT_TEST_CHECK(3 == sum_init(1, 2));

    BOOST_FIT_STATIC_TEST_CHECK(3 == sum_init(1, 2));
}

BOOST_FIT_STATIC_FUNCTION(sum_partial) = boost::fit::partial(sum_f());
BOOST_FIT_TEST_CASE()
{
#ifndef _MSC_VER
    STATIC_ASSERT_EMPTY(sum_partial);
#endif
    BOOST_FIT_TEST_CHECK(3 == sum_partial(1, 2));
    BOOST_FIT_TEST_CHECK(3 == sum_partial(1)(2));

    BOOST_FIT_STATIC_TEST_CHECK(3 == sum_partial(1, 2));
    BOOST_FIT_STATIC_TEST_CHECK(3 == sum_partial(1)(2));
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

BOOST_FIT_STATIC_FUNCTION(sum_partial) = boost::fit::partial(sum_f());

BOOST_FIT_TEST_CASE()
{
#ifndef _MSC_VER
    STATIC_ASSERT_EMPTY(sum_partial);
#endif
    BOOST_FIT_TEST_CHECK(3 == sum_partial(1, 2));
    BOOST_FIT_TEST_CHECK(3 == sum_partial(1)(2));
}

BOOST_FIT_STATIC_FUNCTION(add_one_pipable) = boost::fit::pipable(add_one_f());

BOOST_FIT_TEST_CASE()
{
// TODO: Make this work on msvc
#ifndef _MSC_VER
    STATIC_ASSERT_EMPTY(add_one_pipable);
#endif
    BOOST_FIT_TEST_CHECK(3 == add_one_pipable(2));
    BOOST_FIT_TEST_CHECK(3 == (2 | add_one_pipable));
}

BOOST_FIT_STATIC_FUNCTION(sum_infix) = boost::fit::infix(sum_f());

BOOST_FIT_TEST_CASE()
{
// TODO: Make this work on msvc
#ifndef _MSC_VER
    STATIC_ASSERT_EMPTY(sum_infix);
#endif
    BOOST_FIT_TEST_CHECK(3 == (1 <sum_infix> 2));
}

}
