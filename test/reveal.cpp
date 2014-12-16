#include "test.h"
#include <fit/reveal.h>
#include <fit/conditional.h>
#include <fit/static.h>
#include <fit/lambda.h>

namespace reveal_test {

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

fit::static_<fit::conditional_adaptor<f1, f2, f3> > f = {}; 

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::reveal(f)(t1()) == 1);
    FIT_TEST_CHECK(fit::reveal(f)(t2()) == 2);
    FIT_TEST_CHECK(fit::reveal(f)(t3()) == 3);
    // fit::reveal(f)(1);
}

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
    FIT_TEST_CHECK(fit::reveal(lam)(t1()) == 1);
    FIT_TEST_CHECK(fit::reveal(lam)(t2()) == 2);
    FIT_TEST_CHECK(fit::reveal(lam)(t3()) == 3);

    // fit::reveal(lam)(1);
}

FIT_STATIC_FUNCTION(static_fun) = fit::conditional(
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
    FIT_TEST_CHECK(fit::reveal(static_fun)(t1()) == 1);
    FIT_TEST_CHECK(fit::reveal(static_fun)(t2()) == 2);
    FIT_TEST_CHECK(fit::reveal(static_fun)(t3()) == 3);

    // fit::reveal(static_fun)(1);
}

}