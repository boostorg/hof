#include <boost/fit/arg.hpp>
#include <type_traits>
#include "test.hpp"

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::arg_c<3>(1,2,3,4,5) == 3);
    BOOST_FIT_TEST_CHECK( boost::fit::arg_c<3>(1,2,3,4,5) == 3 );
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::arg(std::integral_constant<int, 3>())(1,2,3,4,5) == 3);
    BOOST_FIT_TEST_CHECK( boost::fit::arg(std::integral_constant<int, 3>())(1,2,3,4,5) == 3 );
}
