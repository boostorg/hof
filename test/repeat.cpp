#include <boost/fit/repeat.hpp>
#include "test.hpp"


struct increment
{
    template<class T>
    constexpr T operator()(T x) const
    {
        return x + 1;
    }
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::repeat(std::integral_constant<int, 5>())(increment())(1) == 6);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::repeat(std::integral_constant<int, 5>())(increment())(1) == 6);
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::repeat(5)(increment())(1) == 6);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::repeat(5)(increment())(1) == 6);
}
