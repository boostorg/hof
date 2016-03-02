#include <boost/fit/limit.hpp>
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
