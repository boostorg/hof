#include <boost/fit/identity.hpp>
#include "test.hpp"

BOOST_FIT_TEST_CASE()
{
	BOOST_FIT_STATIC_TEST_CHECK(boost::fit::identity(10) == 10);
	BOOST_FIT_TEST_CHECK(boost::fit::identity(10) == 10);
}
