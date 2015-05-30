#include <fit/conditional.h>
#include <fit/static.h>
#include <fit/lambda.h>
#include <fit/function.h>
#include <memory>
#include "test.h"

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
    f_move ## n() : i(new int(n)) {}; \
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

fit::static_<fit::conditional_adaptor<f1, f2, f3, ff> > f = {}; 

FIT_STATIC_AUTO f_constexpr = fit::conditional_adaptor<f1, f2, f3, ff>();

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(f(t1()) == 1);
    FIT_TEST_CHECK(f(t2()) == 2);
    FIT_TEST_CHECK(f(t3()) == 3);

    FIT_STATIC_TEST_CHECK(f_constexpr(t1()) == 1);
    FIT_STATIC_TEST_CHECK(f_constexpr(t2()) == 2);
    FIT_STATIC_TEST_CHECK(f_constexpr(t3()) == 3);
}

FIT_TEST_CASE()
{
    auto f_move_local = fit::conditional(f_move1(), f_move2(), f_move3());
    STATIC_ASSERT_MOVE_ONLY(decltype(f_move_local));
    FIT_TEST_CHECK(f_move_local(t_move1()) == 1);
    FIT_TEST_CHECK(f_move_local(t_move2()) == 2);
    FIT_TEST_CHECK(f_move_local(t_move3()) == 3);
}
#ifndef _MSC_VER
static constexpr auto lam = fit::conditional(
    FIT_STATIC_LAMBDA(t1)
    {
        return 1;
    },
    FIT_STATIC_LAMBDA(t2)
    {
        return 2;
    },
    FIT_STATIC_LAMBDA(t3)
    {
        return 3;
    }
);

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(lam(t1()) == 1);
    FIT_TEST_CHECK(lam(t2()) == 2);
    FIT_TEST_CHECK(lam(t3()) == 3);
}
#endif

FIT_STATIC_LAMBDA_FUNCTION(static_fun) = fit::conditional(
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

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(static_fun(t1()) == 1);
    FIT_TEST_CHECK(static_fun(t2()) == 2);
    FIT_TEST_CHECK(static_fun(t3()) == 3);
}

}
