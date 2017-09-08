#include <boost/fit/flip.hpp>
#include <boost/fit/placeholders.hpp>
#include "test.hpp"

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == boost::fit::flip(boost::fit::_ - boost::fit::_)(2, 5));
    BOOST_FIT_STATIC_TEST_CHECK(3 == boost::fit::flip(boost::fit::_ - boost::fit::_)(2, 5));
}

BOOST_FIT_TEST_CASE()
{
    typedef std::integral_constant<int, 1> one;
    typedef std::integral_constant<int, 2> two;
    typedef std::integral_constant<int, 3> three;
    BOOST_FIT_TEST_CHECK(1 == boost::fit::arg(one{})(1, 2, 3, 4));
    BOOST_FIT_STATIC_TEST_CHECK(1 == boost::fit::arg(one{})(1, 2, 3, 4));
    BOOST_FIT_TEST_CHECK(2 == boost::fit::flip(boost::fit::arg(one{}))(1, 2, 3, 4));
    BOOST_FIT_STATIC_TEST_CHECK(2 == boost::fit::flip(boost::fit::arg(one{}))(1, 2, 3, 4));

    BOOST_FIT_TEST_CHECK(2 == boost::fit::arg(two{})(1, 2, 3, 4));
    BOOST_FIT_STATIC_TEST_CHECK(2 == boost::fit::arg(two{})(1, 2, 3, 4));
    BOOST_FIT_TEST_CHECK(1 == boost::fit::flip(boost::fit::arg(two{}))(1, 2, 3, 4));
    BOOST_FIT_STATIC_TEST_CHECK(1 == boost::fit::flip(boost::fit::arg(two{}))(1, 2, 3, 4));

    BOOST_FIT_TEST_CHECK(3 == boost::fit::arg(three{})(1, 2, 3, 4));
    BOOST_FIT_STATIC_TEST_CHECK(3 == boost::fit::arg(three{})(1, 2, 3, 4));
    BOOST_FIT_TEST_CHECK(3 == boost::fit::flip(boost::fit::arg(three{}))(1, 2, 3, 4));
    BOOST_FIT_STATIC_TEST_CHECK(3 == boost::fit::flip(boost::fit::arg(three{}))(1, 2, 3, 4));
}

#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define FINAL
#else
#define FINAL final
#endif


struct f FINAL {
    int operator()(int i, void *) const {
        return i;
    }
};

#if BOOST_FIT_HAS_NOEXCEPT_DEDUCTION
BOOST_FIT_TEST_CASE()
{
    static_assert(boost::fit::flip(boost::fit::_ - boost::fit::_)(2, 5), "noexcept flip");
}
#endif

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::flip(f())(nullptr, 2) == 2);
}
