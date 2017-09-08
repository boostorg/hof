#include <boost/fit/tap.hpp>
#include "test.hpp"

struct sum_f
{
    template<class T, class U>
    constexpr T operator()(T x, U y) const
    {
        return x+y;
    }
};

static constexpr boost::fit::pipable_adaptor<sum_f> sum = {};
// TODO: Test constexpr
BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == (1 | sum(2)));
    BOOST_FIT_TEST_CHECK(5 == (1 | sum(2) | boost::fit::tap([](int i) { BOOST_FIT_TEST_CHECK(3 == i); }) | sum(2)));
}
