#include <fit/compose.hpp>
#include <fit/function.hpp>
#include <fit/lambda.hpp>
#include <memory>
#include "test.hpp"

namespace compose_test {
struct increment
{
    template<class T>
    constexpr T operator()(T x) const noexcept
    {
        return x + 1;
    }
};

struct decrement
{
    template<class T>
    constexpr T operator()(T x) const noexcept
    {
        return x - 1;
    }
};

struct negate
{
    template<class T>
    constexpr T operator()(T x) const noexcept
    {
        return -x;
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

#if FIT_HAS_NOEXCEPT_DEDUCTION
FIT_TEST_CASE()
{
    static_assert(noexcept(fit::compose(increment(), decrement(), increment())(3)), "noexcept compose");
}
#endif

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::compose(fit::identity)(3) == 3);
    FIT_TEST_CHECK(fit::compose(fit::identity, fit::identity)(3) == 3);
    FIT_TEST_CHECK(fit::compose(fit::identity, fit::identity, fit::identity)(3) == 3);

    FIT_STATIC_TEST_CHECK(fit::compose(fit::identity)(3) == 3);
    FIT_STATIC_TEST_CHECK(fit::compose(fit::identity, fit::identity)(3) == 3);
    FIT_STATIC_TEST_CHECK(fit::compose(fit::identity, fit::identity, fit::identity)(3) == 3);
}

FIT_TEST_CASE()
{
    int r = fit::compose(increment(), decrement(), increment())(3);
    FIT_TEST_CHECK(r == 4);
    FIT_STATIC_TEST_CHECK(fit::compose(increment(), decrement(), increment())(3) == 4);
}

FIT_TEST_CASE()
{
    int r = fit::compose(increment(), negate(), decrement(), decrement())(3);
    FIT_TEST_CHECK(r == 0);
    FIT_STATIC_TEST_CHECK(fit::compose(increment(), negate(), decrement(), decrement())(3) == 0);
}
FIT_TEST_CASE()
{
    constexpr auto f = fit::compose(increment(), decrement());
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(f)>::value, "Compose function not empty");
#endif
    static_assert(FIT_IS_DEFAULT_CONSTRUCTIBLE(decltype(f)), "Compose function not default constructible");
    int r = f(3);
    FIT_TEST_CHECK(r == 3);
    FIT_STATIC_TEST_CHECK(f(3) == 3);
}

#ifndef _MSC_VER
FIT_TEST_CASE()
{
    constexpr auto f = fit::compose(increment(), negate(), decrement(), decrement());
    static_assert(std::is_empty<decltype(f)>::value, "Compose function not empty");
    static_assert(FIT_IS_DEFAULT_CONSTRUCTIBLE(decltype(f)), "Compose function not default constructible");
    int r = f(3);
    FIT_TEST_CHECK(r == 0);
    FIT_STATIC_TEST_CHECK(f(3) == 0);
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


FIT_STATIC_FUNCTION(f_compose_single_function) = fit::compose(increment());

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(f_compose_single_function(3) == 4);
    FIT_STATIC_TEST_CHECK(f_compose_single_function(3) == 4);
}

FIT_STATIC_FUNCTION(f_compose_function) = fit::compose(increment(), decrement(), increment());

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(f_compose_function(3) == 4);
    FIT_STATIC_TEST_CHECK(f_compose_function(3) == 4);
}

FIT_STATIC_FUNCTION(f_compose_function_4) = fit::compose(increment(), negate(), decrement(), decrement());

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(f_compose_function_4(3) == 0);
    FIT_STATIC_TEST_CHECK(f_compose_function_4(3) == 0);
}

FIT_STATIC_LAMBDA_FUNCTION(f_compose_lambda) = fit::compose(
    [](int i) { return i+1; }, 
    [](int i) { return i-1; }, 
    [](int i) { return i+1; }
);

FIT_TEST_CASE()
{
    int r = f_compose_lambda(3);
    FIT_TEST_CHECK(r == 4);
}
}
