#include <boost/fit/compose.hpp>
#include <boost/fit/function.hpp>
#include <boost/fit/lambda.hpp>
#include <memory>
#include "test.hpp"

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

struct negate
{
    template<class T>
    constexpr T operator()(T x) const
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

BOOST_FIT_TEST_CASE()
{
    int r = boost::fit::compose(increment(), decrement(), increment())(3);
    BOOST_FIT_TEST_CHECK(r == 4);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::compose(increment(), decrement(), increment())(3) == 4);
}

BOOST_FIT_TEST_CASE()
{
    int r = boost::fit::compose(increment(), negate(), decrement(), decrement())(3);
    BOOST_FIT_TEST_CHECK(r == 0);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::compose(increment(), negate(), decrement(), decrement())(3) == 0);
}
BOOST_FIT_TEST_CASE()
{
    constexpr auto f = boost::fit::compose(increment(), decrement());
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(f)>::value, "Compose function not empty");
#endif
    static_assert(BOOST_FIT_IS_DEFAULT_CONSTRUCTIBLE(decltype(f)), "Compose function not default constructible");
    int r = f(3);
    BOOST_FIT_TEST_CHECK(r == 3);
    BOOST_FIT_STATIC_TEST_CHECK(f(3) == 3);
}

#ifndef _MSC_VER
BOOST_FIT_TEST_CASE()
{
    constexpr auto f = boost::fit::compose(increment(), negate(), decrement(), decrement());
    static_assert(std::is_empty<decltype(f)>::value, "Compose function not empty");
    static_assert(BOOST_FIT_IS_DEFAULT_CONSTRUCTIBLE(decltype(f)), "Compose function not default constructible");
    int r = f(3);
    BOOST_FIT_TEST_CHECK(r == 0);
    BOOST_FIT_STATIC_TEST_CHECK(f(3) == 0);
}
#endif

BOOST_FIT_TEST_CASE()
{
    STATIC_ASSERT_MOVE_ONLY(increment_movable);
    STATIC_ASSERT_MOVE_ONLY(decrement_movable);
    int r = boost::fit::compose(increment_movable(), decrement_movable(), increment_movable())(3);
    BOOST_FIT_TEST_CHECK(r == 4);
}

BOOST_FIT_TEST_CASE()
{
    const auto f = boost::fit::compose([](int i) { return i+1; }, [](int i) { return i-1; }, [](int i) { return i+1; });
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(f)>::value, "Compose function not empty");
#endif
    int r = f(3);
    BOOST_FIT_TEST_CHECK(r == 4);
}


BOOST_FIT_STATIC_FUNCTION(f_compose_single_function) = boost::fit::compose(increment());

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(f_compose_single_function(3) == 4);
    BOOST_FIT_STATIC_TEST_CHECK(f_compose_single_function(3) == 4);
}

BOOST_FIT_STATIC_FUNCTION(f_compose_function) = boost::fit::compose(increment(), decrement(), increment());

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(f_compose_function(3) == 4);
    BOOST_FIT_STATIC_TEST_CHECK(f_compose_function(3) == 4);
}

BOOST_FIT_STATIC_FUNCTION(f_compose_function_4) = boost::fit::compose(increment(), negate(), decrement(), decrement());

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(f_compose_function_4(3) == 0);
    BOOST_FIT_STATIC_TEST_CHECK(f_compose_function_4(3) == 0);
}

BOOST_FIT_STATIC_LAMBDA_FUNCTION(f_compose_lambda) = boost::fit::compose(
    [](int i) { return i+1; }, 
    [](int i) { return i-1; }, 
    [](int i) { return i+1; }
);

BOOST_FIT_TEST_CASE()
{
    int r = f_compose_lambda(3);
    BOOST_FIT_TEST_CHECK(r == 4);
}
}
