#include <boost/fit/alias.hpp>
#include "test.hpp"

struct foo
{
    int i;
    foo(int i_) : i(i_)
    {}
};

BOOST_FIT_TEST_CASE()
{
    boost::fit::alias<int> ai = 5;
    BOOST_FIT_TEST_CHECK(boost::fit::alias_value(ai) == 5);
    boost::fit::alias_inherit<foo> af = foo{5};
    BOOST_FIT_TEST_CHECK(boost::fit::alias_value(af).i == 5);
}

