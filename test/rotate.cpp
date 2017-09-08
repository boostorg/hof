#include <boost/fit/rotate.hpp>
#include <boost/fit/placeholders.hpp>
#include <boost/fit/compose.hpp>
#include <boost/fit/repeat.hpp>
#include "test.hpp"

struct head
{
    template<class T, class... Ts>
    constexpr T operator()(T x, Ts&&...) const
    BOOST_FIT_RETURNS_DEDUCE_NOEXCEPT(x)
    {
        return x;
    }
};

#if BOOST_FIT_HAS_NOEXCEPT_DEDUCTION
BOOST_FIT_TEST_CASE()
{
    static_assert(noexcept(boost::fit::rotate(head{})(1, 2, 3, 4)), "noexcept rotate");
    static_assert(noexcept(boost::fit::repeat(std::integral_constant<int, 5>{})(boost::fit::rotate)(head{})(1, 2, 3, 4, 5, 6, 7, 8, 9)), "noexcept rotate");
}
#endif

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(2 == boost::fit::rotate(head{})(1, 2, 3, 4));
    BOOST_FIT_STATIC_TEST_CHECK(2 == boost::fit::rotate(head{})(1, 2, 3, 4));
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == boost::fit::compose(boost::fit::rotate, boost::fit::rotate)(head{})(1, 2, 3, 4));
    BOOST_FIT_STATIC_TEST_CHECK(3 == boost::fit::compose(boost::fit::rotate, boost::fit::rotate)(head{})(1, 2, 3, 4));
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(6 == boost::fit::repeat(std::integral_constant<int, 5>{})(boost::fit::rotate)(head{})(1, 2, 3, 4, 5, 6, 7, 8, 9));
    BOOST_FIT_STATIC_TEST_CHECK(6 == boost::fit::repeat(std::integral_constant<int, 5>{})(boost::fit::rotate)(head{})(1, 2, 3, 4, 5, 6, 7, 8, 9));
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == boost::fit::rotate(boost::fit::_ - boost::fit::_)(2, 5));
    BOOST_FIT_STATIC_TEST_CHECK(3 == boost::fit::rotate(boost::fit::_ - boost::fit::_)(2, 5));
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

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::rotate(f())(nullptr, 2) == 2);
}
