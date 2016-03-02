#include <boost/fit/conditional.hpp>
#include <boost/fit/static.hpp>
#include <boost/fit/lambda.hpp>
#include <boost/fit/function.hpp>
#include <memory>
#include "test.hpp"

namespace conditional_test {

#define CONDITIONAL_FUNCTION(n) \
struct t ## n {}; \
struct f ## n \
{ \
    constexpr int operator()(t ## n) const \
    { \
        return n; \
    } \
};

CONDITIONAL_FUNCTION(1)
CONDITIONAL_FUNCTION(2)
CONDITIONAL_FUNCTION(3)

#define CONDITIONAL_MOVE_FUNCTION(n) \
struct t_move ## n {}; \
struct f_move ## n \
{ \
    std::unique_ptr<int> i;\
    f_move ## n(int ip) : i(new int(ip)) {}; \
    int operator()(t_move ## n) const \
    { \
        return *i; \
    } \
};

CONDITIONAL_MOVE_FUNCTION(1)
CONDITIONAL_MOVE_FUNCTION(2)
CONDITIONAL_MOVE_FUNCTION(3)

struct ff
{
    constexpr int operator()(t2) const
    {
        return 500;
    }
};

static constexpr boost::fit::static_<boost::fit::conditional_adaptor<f1, f2, f3, ff> > f = {};

BOOST_FIT_STATIC_FUNCTION(f_constexpr) = boost::fit::conditional_adaptor<f1, f2, f3, ff>();

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(f(t1()) == 1);
    BOOST_FIT_TEST_CHECK(f(t2()) == 2);
    BOOST_FIT_TEST_CHECK(f(t3()) == 3);

    BOOST_FIT_STATIC_TEST_CHECK(f_constexpr(t1()) == 1);
    BOOST_FIT_STATIC_TEST_CHECK(f_constexpr(t2()) == 2);
    BOOST_FIT_STATIC_TEST_CHECK(f_constexpr(t3()) == 3);
}

BOOST_FIT_TEST_CASE()
{
    auto f_move_local = boost::fit::conditional(f_move1(1), f_move2(2), f_move3(3));
    STATIC_ASSERT_MOVE_ONLY(decltype(f_move_local));
    BOOST_FIT_TEST_CHECK(f_move_local(t_move1()) == 1);
    BOOST_FIT_TEST_CHECK(f_move_local(t_move2()) == 2);
    BOOST_FIT_TEST_CHECK(f_move_local(t_move3()) == 3);
}
#ifndef _MSC_VER
static constexpr auto lam = boost::fit::conditional(
    BOOST_FIT_STATIC_LAMBDA(t1)
    {
        return 1;
    },
    BOOST_FIT_STATIC_LAMBDA(t2)
    {
        return 2;
    },
    BOOST_FIT_STATIC_LAMBDA(t3)
    {
        return 3;
    }
);

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(lam(t1()) == 1);
    BOOST_FIT_TEST_CHECK(lam(t2()) == 2);
    BOOST_FIT_TEST_CHECK(lam(t3()) == 3);
}
#endif
#if BOOST_FIT_HAS_STATIC_LAMBDA
BOOST_FIT_STATIC_LAMBDA_FUNCTION(static_fun) = boost::fit::conditional(
    [](t1)
    {
        return 1;
    },
    [](t2)
    {
        return 2;
    },
    [](t3)
    {
        return 3;
    }
);

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(static_fun(t1()) == 1);
    BOOST_FIT_TEST_CHECK(static_fun(t2()) == 2);
    BOOST_FIT_TEST_CHECK(static_fun(t3()) == 3);
}
#endif
}
