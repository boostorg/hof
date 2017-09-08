#include <boost/fit/infix.hpp>
#include <boost/fit/function.hpp>
#include <boost/fit/lambda.hpp>
#include <boost/fit/pipable.hpp>
#include <boost/fit/placeholders.hpp>
#include "test.hpp"

struct sum_f
{
    template<class T, class U>
    constexpr T operator()(T x, U y) const BOOST_FIT_RETURNS_DEDUCE_NOEXCEPT(x+y)
    {
        return x+y;
    }
};

static constexpr boost::fit::infix_adaptor<sum_f> sum = {};

#if BOOST_FIT_HAS_NOEXCEPT_DEDUCTION
BOOST_FIT_TEST_CASE()
{
    static_assert(noexcept(1 <sum> 2), "noexcept infix");
    static_assert(!noexcept(std::string() <sum> std::string()), "noexcept infix");
}
#endif

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == (1 <sum> 2));
    BOOST_FIT_STATIC_TEST_CHECK(3 == (1 <sum> 2));
    
    BOOST_FIT_TEST_CHECK(3 == (sum(1, 2)));
    BOOST_FIT_STATIC_TEST_CHECK(3 == (sum(1, 2)));
}

BOOST_FIT_STATIC_FUNCTION(sum1) = boost::fit::infix(sum_f());

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == (1 <sum1> 2));
    BOOST_FIT_STATIC_TEST_CHECK(3 == (1 <sum1> 2));

    BOOST_FIT_TEST_CHECK(3 == (sum1(1, 2)));
    BOOST_FIT_STATIC_TEST_CHECK(3 == (sum1(1, 2)));
}

BOOST_FIT_STATIC_LAMBDA_FUNCTION(sum2) = boost::fit::infix([](int x, int y) { return x + y; });

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == (1 <sum2> 2));

    BOOST_FIT_TEST_CHECK(3 == (sum2(1, 2)));
}

BOOST_FIT_STATIC_FUNCTION(sum3) = boost::fit::infix(boost::fit::_ + boost::fit::_);

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == (1 <sum3> 2));
    BOOST_FIT_STATIC_TEST_CHECK(3 == (1 <sum3> 2));

    BOOST_FIT_TEST_CHECK(3 == (sum3(1, 2)));
    BOOST_FIT_STATIC_TEST_CHECK(3 == (sum3(1, 2)));
}


BOOST_FIT_STATIC_LAMBDA_FUNCTION(sum4) = boost::fit::infix(boost::fit::infix([](int x, int y) { return x + y; }));

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == (1 <sum4> 2));

    BOOST_FIT_TEST_CHECK(3 == (sum4(1, 2)));
}

BOOST_FIT_STATIC_FUNCTION(sum5) = boost::fit::infix(boost::fit::infix(boost::fit::_ + boost::fit::_));

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == (1 <sum5> 2));
    BOOST_FIT_STATIC_TEST_CHECK(3 == (1 <sum5> 2));

    BOOST_FIT_TEST_CHECK(3 == (sum5(1, 2)));
    BOOST_FIT_STATIC_TEST_CHECK(3 == (sum5(1, 2)));
}

BOOST_FIT_TEST_CASE()
{
#if (defined(__GNUC__) && !defined (__clang__))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses"
#endif
    BOOST_FIT_TEST_CHECK(6 == (1 + 2 <sum> 3));
    BOOST_FIT_TEST_CHECK(3 == 1 <sum> 2);
#if (defined(__GNUC__) && !defined (__clang__))
#pragma GCC diagnostic pop
#endif
}

struct foo {};

BOOST_FIT_TEST_CASE()
{
    auto f = boost::fit::infix([](int, int) { return foo{}; });
    auto g = boost::fit::infix([](foo, foo) { return std::string("hello"); });
    BOOST_FIT_TEST_CHECK((1 <f> 2 <g> foo{}) == "hello");

}
