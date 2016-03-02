#include "test.hpp"
#include <boost/fit/capture.hpp>

// TODO: Test empty capture

BOOST_FIT_TEST_CASE()
{    
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::capture(1, 2)(binary_class())() == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture(1, 2)(binary_class())() == 3);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::capture(1)(binary_class())(2) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture(1)(binary_class())(2) == 3);

    static const int one = 1;
    static const int two = 2;
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::capture_forward(one, two)(binary_class())() == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture_forward(one, two)(binary_class())() == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture_forward(1, 2)(binary_class())() == 3);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::capture_forward(one)(binary_class())(two) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture_forward(1)(binary_class())(2) == 3);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::capture_decay(1, 2)(binary_class())() == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture_decay(1, 2)(binary_class())() == 3);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::capture_decay(1)(binary_class())(2) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture_decay(1)(binary_class())(2) == 3);
}

struct add_member
{
    int i;

    add_member(int ip) : i(ip)
    {}

    int add(int j) const
    {
        return i + j;
    }
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::capture(add_member(1), 2)(&add_member::add)() == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture(add_member(1))(&add_member::add)(2) == 3);
}

