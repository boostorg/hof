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
    static_assert(fit::compose(increment(), decrement(), increment())(3) == 4, "static compose failed");
}

FIT_TEST_CASE()
{
    static_assert(!std::is_copy_constructible<increment_movable>::value && std::is_move_constructible<increment_movable>::value, "Not movable");
    static_assert(!std::is_copy_constructible<decrement_movable>::value && std::is_move_constructible<decrement_movable>::value, "Not movable");
    int r = fit::compose(increment_movable(), decrement_movable(), increment_movable())(3);
    FIT_TEST_CHECK(r == 4);
}

FIT_TEST_CASE()
{
    int r = fit::compose([](int i) { return i+1; }, [](int i) { return i-1; }, [](int i) { return i+1; })(3);
    FIT_TEST_CHECK(r == 4);
}
}
