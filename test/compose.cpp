#include <fit/compose.h>
#include "test.h"

namespace compose_test {
struct increment
{
    template<class T>
    constexpr T operator()(T x) const
    {
        return x + 1;
    }
};

struct decrement
{
    template<class T>
    constexpr T operator()(T x) const
    {
        return x - 1;
    }
};

FIT_TEST_CASE()
{
    int r = fit::compose(increment(), decrement(), increment())(3);
    FIT_TEST_CHECK(r == 4);
    static_assert(fit::compose(increment(), decrement(), increment())(3) == 4, "static compose failed");
}

FIT_TEST_CASE()
{
    int r = fit::compose([](int i) { return i+1; }, [](int i) { return i-1; }, [](int i) { return i+1; })(3);
    FIT_TEST_CHECK(r == 4);
}
}
