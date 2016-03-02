#include <boost/fit/if.hpp>
#include "test.hpp"

#include <boost/fit/conditional.hpp>
#include <boost/fit/placeholders.hpp>


struct is_5
{
    template<class T>
    constexpr bool operator()(T i) const
    {
        return i == 5;
    }
};

struct is_not_5
{
    template<class T>
    constexpr bool operator()(T i) const
    {
        return i != 5;
    }
};

template<class F>
struct test_int
{
    template<class T>
    constexpr bool operator()(T x) const
    {
        return boost::fit::conditional(
            boost::fit::if_(std::is_integral<T>())(F()),
            boost::fit::always(true)
        )(x);
    }
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(test_int<is_5>()(5));
    BOOST_FIT_TEST_CHECK(test_int<is_5>()(5L));
    BOOST_FIT_TEST_CHECK(test_int<is_5>()(5.0));
    BOOST_FIT_TEST_CHECK(test_int<is_5>()(6.0));

    BOOST_FIT_TEST_CHECK(test_int<is_not_5>()(6));
    BOOST_FIT_TEST_CHECK(test_int<is_not_5>()(6L));
    BOOST_FIT_TEST_CHECK(test_int<is_not_5>()(5.0));
    BOOST_FIT_TEST_CHECK(test_int<is_not_5>()(6.0));

    BOOST_FIT_STATIC_TEST_CHECK(test_int<is_5>()(5));
    BOOST_FIT_STATIC_TEST_CHECK(test_int<is_5>()(5L));
    BOOST_FIT_STATIC_TEST_CHECK(test_int<is_5>()(5.0));
    BOOST_FIT_STATIC_TEST_CHECK(test_int<is_5>()(6.0));

    BOOST_FIT_STATIC_TEST_CHECK(test_int<is_not_5>()(6));
    BOOST_FIT_STATIC_TEST_CHECK(test_int<is_not_5>()(6L));
    BOOST_FIT_STATIC_TEST_CHECK(test_int<is_not_5>()(5.0));
    BOOST_FIT_STATIC_TEST_CHECK(test_int<is_not_5>()(6.0));
}

template<class F>
struct test_int_c
{
    template<class T>
    constexpr bool operator()(T x) const
    {
        return boost::fit::conditional(
            boost::fit::if_c<std::is_integral<T>::value>(F()),
            boost::fit::always(true)
        )(x);
    }
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(test_int_c<is_5>()(5));
    BOOST_FIT_TEST_CHECK(test_int_c<is_5>()(5L));
    BOOST_FIT_TEST_CHECK(test_int_c<is_5>()(5.0));
    BOOST_FIT_TEST_CHECK(test_int_c<is_5>()(6.0));

    BOOST_FIT_TEST_CHECK(test_int_c<is_not_5>()(6));
    BOOST_FIT_TEST_CHECK(test_int_c<is_not_5>()(6L));
    BOOST_FIT_TEST_CHECK(test_int_c<is_not_5>()(5.0));
    BOOST_FIT_TEST_CHECK(test_int_c<is_not_5>()(6.0));

    BOOST_FIT_STATIC_TEST_CHECK(test_int_c<is_5>()(5));
    BOOST_FIT_STATIC_TEST_CHECK(test_int_c<is_5>()(5L));
    BOOST_FIT_STATIC_TEST_CHECK(test_int_c<is_5>()(5.0));
    BOOST_FIT_STATIC_TEST_CHECK(test_int_c<is_5>()(6.0));

    BOOST_FIT_STATIC_TEST_CHECK(test_int_c<is_not_5>()(6));
    BOOST_FIT_STATIC_TEST_CHECK(test_int_c<is_not_5>()(6L));
    BOOST_FIT_STATIC_TEST_CHECK(test_int_c<is_not_5>()(5.0));
    BOOST_FIT_STATIC_TEST_CHECK(test_int_c<is_not_5>()(6.0));
}

struct sum_f
{
    template<class T>
    constexpr int operator()(T x, T y) const
    {
        return boost::fit::conditional(
            boost::fit::if_(std::is_integral<T>())(boost::fit::_ + boost::fit::_),
            boost::fit::always(0)
        )(x, y);
    }
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(sum_f()(1, 2) == 3);
    BOOST_FIT_TEST_CHECK(sum_f()(1.0, 2.0) == 0);
    BOOST_FIT_TEST_CHECK(sum_f()("", "") == 0);

    BOOST_FIT_STATIC_TEST_CHECK(sum_f()(1, 2) == 3);
    BOOST_FIT_STATIC_TEST_CHECK(sum_f()("", "") == 0);
}


struct sum_f_c
{
    template<class T>
    constexpr int operator()(T x, T y) const
    {
        return boost::fit::conditional(
            boost::fit::if_c<std::is_integral<T>::value>(boost::fit::_ + boost::fit::_),
            boost::fit::always(0)
        )(x, y);
    }
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(sum_f_c()(1, 2) == 3);
    BOOST_FIT_TEST_CHECK(sum_f_c()(1.0, 2.0) == 0);
    BOOST_FIT_TEST_CHECK(sum_f_c()("", "") == 0);

    BOOST_FIT_STATIC_TEST_CHECK(sum_f_c()(1, 2) == 3);
    BOOST_FIT_STATIC_TEST_CHECK(sum_f_c()("", "") == 0);
}


