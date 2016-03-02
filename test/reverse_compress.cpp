#include <boost/fit/reverse_compress.hpp>
#include "test.hpp"

struct max_f
{
    template<class T, class U>
    constexpr T operator()(T x, U y) const
    {
        return x > y ? x : y;
    }
};

struct sum_f
{
    template<class T, class U>
    constexpr T operator()(T x, U y) const
    {
        return x + y;
    }
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::reverse_compress(max_f(), 0)(2, 3, 4, 5) == 5);
    BOOST_FIT_TEST_CHECK(boost::fit::reverse_compress(max_f(), 0)(5, 4, 3, 2) == 5);
    BOOST_FIT_TEST_CHECK(boost::fit::reverse_compress(max_f(), 0)(2, 3, 5, 4) == 5);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::reverse_compress(max_f(), 0)(2, 3, 4, 5) == 5);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::reverse_compress(max_f(), 0)(5, 4, 3, 2) == 5);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::reverse_compress(max_f(), 0)(2, 3, 5, 4) == 5);
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::reverse_compress(max_f(), 0)() == 0);
    BOOST_FIT_TEST_CHECK(boost::fit::reverse_compress(max_f(), 0)(5) == 5);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::reverse_compress(max_f(), 0)() == 0);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::reverse_compress(max_f(), 0)(5) == 5);
}

template<class... Ts>
constexpr auto find_positive_max(Ts... xs) BOOST_FIT_RETURNS
(
    boost::fit::reverse_compress(max_f(), 0)(xs...)
);

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(find_positive_max() == 0);
    BOOST_FIT_TEST_CHECK(find_positive_max(5) == 5);

    BOOST_FIT_STATIC_TEST_CHECK(find_positive_max() == 0);
    BOOST_FIT_STATIC_TEST_CHECK(find_positive_max(5) == 5);
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::reverse_compress(max_f())(5) == 5);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::reverse_compress(max_f())(5) == 5);
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::reverse_compress(max_f())(2, 3, 4, 5) == 5);
    BOOST_FIT_TEST_CHECK(boost::fit::reverse_compress(max_f())(5, 4, 3, 2) == 5);
    BOOST_FIT_TEST_CHECK(boost::fit::reverse_compress(max_f())(2, 3, 5, 4) == 5);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::reverse_compress(max_f())(2, 3, 4, 5) == 5);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::reverse_compress(max_f())(5, 4, 3, 2) == 5);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::reverse_compress(max_f())(2, 3, 5, 4) == 5);
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::reverse_compress(sum_f(), std::string())("hello", "-", "world") == "world-hello");
}
