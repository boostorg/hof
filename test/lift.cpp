#include "test.hpp"
#include <boost/fit/lift.hpp>
#include <boost/fit/function.hpp>
#include <boost/fit/detail/move.hpp>
#include <tuple>
#include <algorithm>

template<class T, class U>
constexpr T sum(T x, U y)
{
    return x + y;
}

BOOST_FIT_LIFT_CLASS(max_f, std::max);
BOOST_FIT_LIFT_CLASS(sum_f, sum);

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(max_f()(3, 4) == std::max(3, 4));

    BOOST_FIT_TEST_CHECK(sum_f()(1, 2) == 3);
    BOOST_FIT_STATIC_TEST_CHECK(sum_f()(1, 2) == 3);
}

#if BOOST_FIT_HAS_GENERIC_LAMBDA
BOOST_FIT_TEST_CASE()
{
    auto my_max = BOOST_FIT_LIFT(std::max);
    BOOST_FIT_TEST_CHECK(my_max(3, 4) == std::max(3, 4));
    
    BOOST_FIT_TEST_CHECK(BOOST_FIT_LIFT(std::max)(3, 4) == std::max(3, 4));
    BOOST_FIT_TEST_CHECK(BOOST_FIT_LIFT(sum)(1, 2) == 3);
}

BOOST_FIT_STATIC_FUNCTION(psum) = BOOST_FIT_LIFT(sum);
BOOST_FIT_STATIC_FUNCTION(pmax) = BOOST_FIT_LIFT(std::max);

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(pmax(3, 4) == std::max(3, 4));

    BOOST_FIT_TEST_CHECK(psum(1, 2) == 3);
}
#endif
