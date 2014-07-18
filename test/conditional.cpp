#include <fit/conditional.h>
#include <fit/static.h>
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

struct ff
{
    constexpr int operator()(t2) const
    {
        return 500;
    }
};

fit::static_<fit::conditional_adaptor<f1, f2, f3, ff> > f = {}; 

constexpr const auto f_constexpr = fit::conditional_adaptor<f1, f2, f3, ff>();

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(f(t1()) == 1);
    FIT_TEST_CHECK(f(t2()) == 2);
    FIT_TEST_CHECK(f(t3()) == 3);

    static_assert(f_constexpr(t1()) == 1, "static conditional failed");
    static_assert(f_constexpr(t2()) == 2, "static conditional failed");
    static_assert(f_constexpr(t3()) == 3, "static conditional failed");
}
}