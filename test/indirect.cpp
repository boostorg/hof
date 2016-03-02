#include <boost/fit/indirect.hpp>
#include "test.hpp"

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == boost::fit::indirect(std::unique_ptr<binary_class>(new binary_class()))(1, 2));
    BOOST_FIT_TEST_CHECK(3 == boost::fit::reveal(boost::fit::indirect(std::unique_ptr<binary_class>(new binary_class())))(1, 2));

    binary_class f;

    BOOST_FIT_TEST_CHECK(3 == boost::fit::indirect(&f)(1, 2));
    BOOST_FIT_TEST_CHECK(3 == boost::fit::reveal(boost::fit::indirect(&f))(1, 2));
}
