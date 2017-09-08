#include "test.hpp"
#include <boost/fit/reveal.hpp>
#include <boost/fit/conditional.hpp>
#include <boost/fit/static.hpp>
#include <boost/fit/lambda.hpp>
#include <boost/fit/fix.hpp>

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

typedef boost::fit::conditional_adaptor<f1, f2, f3> f_type;
static constexpr boost::fit::static_<f_type> f = {}; 

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::reveal(f)(t1()) == 1);
    BOOST_FIT_TEST_CHECK(boost::fit::reveal(f)(t2()) == 2);
    BOOST_FIT_TEST_CHECK(boost::fit::reveal(f)(t3()) == 3);


    static_assert(boost::fit::is_callable<boost::fit::reveal_adaptor<f_type>, t1>::value, "Callable");
    static_assert(boost::fit::is_callable<boost::fit::reveal_adaptor<f_type>, t2>::value, "Callable");
    static_assert(boost::fit::is_callable<boost::fit::reveal_adaptor<f_type>, t3>::value, "Callable");

    static_assert(!boost::fit::is_callable<boost::fit::reveal_adaptor<f_type>, int>::value, "Callable");
    // boost::fit::reveal(f)(1);
}

#ifndef _MSC_VER
static constexpr auto lam = boost::fit::conditional(
    BOOST_FIT_STATIC_LAMBDA(t1)
    {
        return 1;
    },
    BOOST_FIT_STATIC_LAMBDA(t2)
    {
        return 2;
    },
    BOOST_FIT_STATIC_LAMBDA(t3)
    {
        return 3;
    }
);

BOOST_FIT_TEST_CASE()
{
    STATIC_ASSERT_EMPTY(lam);
    STATIC_ASSERT_EMPTY(boost::fit::reveal(lam));
    BOOST_FIT_TEST_CHECK(boost::fit::reveal(lam)(t1()) == 1);
    BOOST_FIT_TEST_CHECK(boost::fit::reveal(lam)(t2()) == 2);
    BOOST_FIT_TEST_CHECK(boost::fit::reveal(lam)(t3()) == 3);

    // boost::fit::reveal(lam)(1);
    // lam(1);
}
#endif

BOOST_FIT_STATIC_LAMBDA_FUNCTION(static_fun) = boost::fit::conditional(
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

BOOST_FIT_TEST_CASE()
{
#ifndef _MSC_VER
    STATIC_ASSERT_EMPTY(static_fun);
    // STATIC_ASSERT_EMPTY(boost::fit::reveal(static_fun));
#endif
    BOOST_FIT_TEST_CHECK(boost::fit::reveal(static_fun)(t1()) == 1);
    BOOST_FIT_TEST_CHECK(boost::fit::reveal(static_fun)(t2()) == 2);
    BOOST_FIT_TEST_CHECK(boost::fit::reveal(static_fun)(t3()) == 3);

    BOOST_FIT_TEST_CHECK(static_fun(t1()) == 1);
    BOOST_FIT_TEST_CHECK(static_fun(t2()) == 2);
    BOOST_FIT_TEST_CHECK(static_fun(t3()) == 3);

    // boost::fit::reveal(static_fun)(1);
}

struct integral_type
{
    template<class T>
    BOOST_FIT_USING_TYPENAME(failure_alias, std::enable_if<std::is_integral<T>::value>::type);

    struct failure
    : boost::fit::as_failure<failure_alias>
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
    BOOST_FIT_USING_TYPENAME(failure_alias, std::enable_if<!std::is_same<T, dont_catch>::value>::type);

    struct failure
    : boost::fit::as_failure<failure_alias>
    {};

    template<class T, class=typename std::enable_if<!std::is_same<T, dont_catch>::value>::type>
    constexpr int operator()(T) const
    {
        return -1;
    }
};

static constexpr boost::fit::reveal_adaptor<boost::fit::conditional_adaptor<integral_type, catch_all>> check_failure = {}; 


BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(check_failure(5) == 5);
    BOOST_FIT_TEST_CHECK(check_failure(foo()) == -1);

    static_assert(!boost::fit::is_callable<decltype(check_failure), dont_catch>::value, "Callable");
    static_assert(!boost::fit::is_callable<decltype(check_failure), int, int>::value, "Callable");

    // check_failure(dont_catch());
}

}
