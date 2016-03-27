#include "test.hpp"
#include <fit/reveal.hpp>
#include <fit/conditional.hpp>
#include <fit/static.hpp>
#include <fit/lambda.hpp>
#include <fit/fix.hpp>

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

typedef fit::conditional_adaptor<f1, f2, f3> f_type;
static constexpr fit::static_<f_type> f = {}; 

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::reveal(f)(t1()) == 1);
    FIT_TEST_CHECK(fit::reveal(f)(t2()) == 2);
    FIT_TEST_CHECK(fit::reveal(f)(t3()) == 3);


    static_assert(fit::is_callable<fit::reveal_adaptor<f_type>, t1>::value, "Callable");
    static_assert(fit::is_callable<fit::reveal_adaptor<f_type>, t2>::value, "Callable");
    static_assert(fit::is_callable<fit::reveal_adaptor<f_type>, t3>::value, "Callable");

    static_assert(!fit::is_callable<fit::reveal_adaptor<f_type>, int>::value, "Callable");
    // fit::reveal(f)(1);
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
    STATIC_ASSERT_EMPTY(lam);
    STATIC_ASSERT_EMPTY(fit::reveal(lam));
    FIT_TEST_CHECK(fit::reveal(lam)(t1()) == 1);
    FIT_TEST_CHECK(fit::reveal(lam)(t2()) == 2);
    FIT_TEST_CHECK(fit::reveal(lam)(t3()) == 3);

    // fit::reveal(lam)(1);
    // lam(1);
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
#ifndef _MSC_VER
    STATIC_ASSERT_EMPTY(static_fun);
    // STATIC_ASSERT_EMPTY(fit::reveal(static_fun));
#endif
    FIT_TEST_CHECK(fit::reveal(static_fun)(t1()) == 1);
    FIT_TEST_CHECK(fit::reveal(static_fun)(t2()) == 2);
    FIT_TEST_CHECK(fit::reveal(static_fun)(t3()) == 3);

    FIT_TEST_CHECK(static_fun(t1()) == 1);
    FIT_TEST_CHECK(static_fun(t2()) == 2);
    FIT_TEST_CHECK(static_fun(t3()) == 3);

    // fit::reveal(static_fun)(1);
}

struct integral_type
{
    template<class T>
    FIT_USING_TYPENAME(failure_alias, std::enable_if<std::is_integral<T>::value>::type);

    struct failure
    : fit::as_failure<failure_alias>
    {};

    template<class T, class=typename std::enable_if<std::is_integral<T>::value>::type>
    constexpr T operator()(T x) const
    {
        return x;
    }
};
struct foo {};
struct dont_catch {};

struct catch_all
{
    template<class T>
    FIT_USING_TYPENAME(failure_alias, std::enable_if<!std::is_same<T, dont_catch>::value>::type);

    struct failure
    : fit::as_failure<failure_alias>
    {};

    template<class T, class=typename std::enable_if<!std::is_same<T, dont_catch>::value>::type>
    constexpr int operator()(T) const
    {
        return -1;
    }
};

static constexpr fit::reveal_adaptor<fit::conditional_adaptor<integral_type, catch_all>> check_failure = {}; 


FIT_TEST_CASE()
{
    FIT_TEST_CHECK(check_failure(5) == 5);
    FIT_TEST_CHECK(check_failure(foo()) == -1);

    static_assert(!fit::is_callable<decltype(check_failure), dont_catch>::value, "Callable");
    static_assert(!fit::is_callable<decltype(check_failure), int, int>::value, "Callable");

    // check_failure(dont_catch());
}

}
