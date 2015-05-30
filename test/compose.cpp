#include <fit/compose.h>
#include <memory>
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

struct increment_movable
{
    std::unique_ptr<int> n;
    increment_movable()
    : n(new int(1))
    {}
    template<class T>
    T operator()(T x) const
    {
        return x + *n;
    }
};

struct decrement_movable
{
    std::unique_ptr<int> n;
    decrement_movable()
    : n(new int(1))
    {}
    template<class T>
    T operator()(T x) const
    {
        return x - *n;
    }
};

FIT_TEST_CASE()
{
    int r = fit::compose(increment(), decrement(), increment())(3);
    FIT_TEST_CHECK(r == 4);
    FIT_STATIC_TEST_CHECK(fit::compose(increment(), decrement(), increment())(3) == 4);
}
#ifndef _MSC_VER
FIT_TEST_CASE()
{
    constexpr auto f = fit::compose(increment(), decrement());
    static_assert(std::is_empty<decltype(f)>::value, "Compose function not empty");
    int r = f(3);
    FIT_TEST_CHECK(r == 3);
    FIT_STATIC_TEST_CHECK(f(3) == 3);
}
#endif

FIT_TEST_CASE()
{
    STATIC_ASSERT_MOVE_ONLY(increment_movable);
    STATIC_ASSERT_MOVE_ONLY(decrement_movable);
    int r = fit::compose(increment_movable(), decrement_movable(), increment_movable())(3);
    FIT_TEST_CHECK(r == 4);
}

FIT_TEST_CASE()
{
    const auto f = fit::compose([](int i) { return i+1; }, [](int i) { return i-1; }, [](int i) { return i+1; });
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(f)>::value, "Compose function not empty");
#endif
    int r = f(3);
    FIT_TEST_CHECK(r == 4);
}
}
