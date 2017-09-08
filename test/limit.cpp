#include <boost/fit/limit.hpp>
#include <boost/fit/is_callable.hpp>
#include "test.hpp"

BOOST_FIT_TEST_CASE()
{
    auto f = boost::fit::limit(std::integral_constant<int, 2>())(binary_class());
    BOOST_FIT_TEST_CHECK(f(1, 2) == 3);
    static_assert(boost::fit::function_param_limit<decltype(f)>::value == 2, "Function limit is 2");
}

BOOST_FIT_TEST_CASE()
{
    auto f = boost::fit::limit_c<2>(binary_class());
    BOOST_FIT_TEST_CHECK(f(1, 2) == 3);
    static_assert(boost::fit::function_param_limit<decltype(f)>::value == 2, "Function limit is 2");
}

BOOST_FIT_TEST_CASE()
{
    auto f = boost::fit::limit_c<2>(boost::fit::always(3));
    BOOST_FIT_TEST_CHECK(f(1, 2) == 3);
    BOOST_FIT_TEST_CHECK(f(1) == 3);
    BOOST_FIT_TEST_CHECK(f() == 3);
    static_assert(boost::fit::function_param_limit<decltype(f)>::value == 2, "Function limit is 2");
    static_assert(boost::fit::is_callable<decltype(f), int>::value, "Callable");
    static_assert(boost::fit::is_callable<decltype(f), int, int>::value, "Callable");
    static_assert(!boost::fit::is_callable<decltype(f), int, int, int>::value, "Not Callable");
}

BOOST_FIT_TEST_CASE()
{
    static_assert(!boost::fit::is_callable<decltype(boost::fit::limit), int>::value, "Not integral constant");
}

