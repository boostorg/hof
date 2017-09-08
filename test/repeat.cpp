#include <boost/fit/repeat.hpp>
#include <limits>
#include "test.hpp"

// TODO: Add tests for multiple parameters

struct increment
{
    template<class T>
    constexpr T operator()(T x) const noexcept
    {
        return x + 1;
    }
};

#if BOOST_FIT_HAS_NOEXCEPT_DEDUCTION
BOOST_FIT_TEST_CASE()
{
    static_assert(noexcept(boost::fit::repeat(std::integral_constant<int, 5>())(increment())(1)), "noexcept repeat");
    static_assert(noexcept(boost::fit::repeat(5)(increment())(1)), "noexcept repeat");
}
#endif

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

BOOST_FIT_TEST_CASE()
{
    int i = 5;
    BOOST_FIT_TEST_CHECK(boost::fit::repeat(i)(increment())(1) == 6);
}

BOOST_FIT_TEST_CASE()
{
    static const int i = 5;
    BOOST_FIT_TEST_CHECK(boost::fit::repeat(i)(increment())(1) == 6);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::repeat(i)(increment())(1) == 6);
}

// BOOST_FIT_TEST_CASE()
// {
//     BOOST_FIT_TEST_CHECK(boost::fit::repeat(std::numeric_limits<int>::max()/4)(increment())(0) == std::numeric_limits<int>::max()/4);
// }

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::repeat(BOOST_FIT_RECURSIVE_CONSTEXPR_DEPTH+4)(increment())(0) == BOOST_FIT_RECURSIVE_CONSTEXPR_DEPTH+4);
#if BOOST_FIT_HAS_RELAXED_CONSTEXPR
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::repeat(BOOST_FIT_RECURSIVE_CONSTEXPR_DEPTH+4)(increment())(0) == BOOST_FIT_RECURSIVE_CONSTEXPR_DEPTH+4);
#endif
}
