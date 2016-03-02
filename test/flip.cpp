#include <boost/fit/flip.hpp>
#include <boost/fit/placeholders.hpp>
#include "test.hpp"

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == boost::fit::flip(boost::fit::_ - boost::fit::_)(2, 5));
    BOOST_FIT_STATIC_TEST_CHECK(3 == boost::fit::flip(boost::fit::_ - boost::fit::_)(2, 5));
}
