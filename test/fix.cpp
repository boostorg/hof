#include <fit/fix.h>
#include <fit/static.h>
#include "test.h"

struct factorial_t
{
    template<class Self, class T>
    FIT_FIX_CONSTEXPR T operator()(Self s, T x) const
    {
        return x == 0 ? 1 : x * s(x-1);
    }
};

fit::static_<fit::fix_adaptor<factorial_t> > factorial = {};

FIT_TEST_CASE()
{
    const int r1 = factorial(5);
    FIT_TEST_CHECK(r1 == 5*4*3*2*1);
#if FIT_FIX_HAS_CONSTEXPR
    static_assert(r1 == 5*4*3*2*1, "Fix constexpr failed");
#endif
    int r2 = fit::fix([](auto s, auto x) -> decltype(x) { return x == 0 ? 1 : x * s(x-1); })(5);
    FIT_TEST_CHECK(r2 == 5*4*3*2*1);
}