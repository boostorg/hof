#include <boost/fit/protect.hpp>
#include <boost/fit/lazy.hpp>
#include <boost/fit/placeholders.hpp>
#include <memory>
#include "test.hpp"

#include <boost/fit/function.hpp>

int f(int x)
{
    return x;
}

int& g(int& x)
{
    return x;
}

template<class T>
const T& constify(const T& arg)
{
    return arg;
}

BOOST_FIT_TEST_CASE()
{
    int i[9] = {0,1,2,3,4,5,6,7,8};

    // non-const

    // test nullary
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(1))() == 1);

    // test lvalues

    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1))(i[0]) == &i[0]);

    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1))(i[0], i[1]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2))(i[0], i[1]) == &i[1]);

    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1))(i[0], i[1], i[2]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2))(i[0], i[1], i[2]) == &i[1]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_3))(i[0], i[1], i[2]) == &i[2]);

    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1))(i[0], i[1], i[2], i[3]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2))(i[0], i[1], i[2], i[3]) == &i[1]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_3))(i[0], i[1], i[2], i[3]) == &i[2]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_4))(i[0], i[1], i[2], i[3]) == &i[3]);

    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1))(i[0], i[1], i[2], i[3], i[4]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2))(i[0], i[1], i[2], i[3], i[4]) == &i[1]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_3))(i[0], i[1], i[2], i[3], i[4]) == &i[2]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_4))(i[0], i[1], i[2], i[3], i[4]) == &i[3]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_5))(i[0], i[1], i[2], i[3], i[4]) == &i[4]);

    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1))(i[0], i[1], i[2], i[3], i[4], i[5]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2))(i[0], i[1], i[2], i[3], i[4], i[5]) == &i[1]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_3))(i[0], i[1], i[2], i[3], i[4], i[5]) == &i[2]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_4))(i[0], i[1], i[2], i[3], i[4], i[5]) == &i[3]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_5))(i[0], i[1], i[2], i[3], i[4], i[5]) == &i[4]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_6))(i[0], i[1], i[2], i[3], i[4], i[5]) == &i[5]);

    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1))(i[0], i[1], i[2], i[3], i[4], i[5], i[6]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2))(i[0], i[1], i[2], i[3], i[4], i[5], i[6]) == &i[1]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_3))(i[0], i[1], i[2], i[3], i[4], i[5], i[6]) == &i[2]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_4))(i[0], i[1], i[2], i[3], i[4], i[5], i[6]) == &i[3]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_5))(i[0], i[1], i[2], i[3], i[4], i[5], i[6]) == &i[4]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_6))(i[0], i[1], i[2], i[3], i[4], i[5], i[6]) == &i[5]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_7))(i[0], i[1], i[2], i[3], i[4], i[5], i[6]) == &i[6]);

    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[1]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_3))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[2]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_4))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[3]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_5))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[4]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_6))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[5]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_7))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[6]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_8))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[7]);

    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[1]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_3))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[2]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_4))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[3]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_5))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[4]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_6))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[5]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_7))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[6]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_8))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[7]);
    BOOST_FIT_TEST_CHECK(&boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_9))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[8]);

    // test rvalues

    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1))(0) == 0);

    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1))(0, 1) == 0);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2))(0, 1) == 1);

    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1))(0, 1, 2) == 0);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2))(0, 1, 2) == 1);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_3))(0, 1, 2) == 2);

    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1))(0, 1, 2, 3) == 0);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2))(0, 1, 2, 3) == 1);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_3))(0, 1, 2, 3) == 2);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_4))(0, 1, 2, 3) == 3);

    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1))(0, 1, 2, 3, 4) == 0);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2))(0, 1, 2, 3, 4) == 1);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_3))(0, 1, 2, 3, 4) == 2);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_4))(0, 1, 2, 3, 4) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_5))(0, 1, 2, 3, 4) == 4);

    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1))(0, 1, 2, 3, 4, 5) == 0);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2))(0, 1, 2, 3, 4, 5) == 1);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_3))(0, 1, 2, 3, 4, 5) == 2);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_4))(0, 1, 2, 3, 4, 5) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_5))(0, 1, 2, 3, 4, 5) == 4);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_6))(0, 1, 2, 3, 4, 5) == 5);

    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1))(0, 1, 2, 3, 4, 5, 6) == 0);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2))(0, 1, 2, 3, 4, 5, 6) == 1);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_3))(0, 1, 2, 3, 4, 5, 6) == 2);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_4))(0, 1, 2, 3, 4, 5, 6) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_5))(0, 1, 2, 3, 4, 5, 6) == 4);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_6))(0, 1, 2, 3, 4, 5, 6) == 5);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_7))(0, 1, 2, 3, 4, 5, 6) == 6);

    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1))(0, 1, 2, 3, 4, 5, 6, 7) == 0);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2))(0, 1, 2, 3, 4, 5, 6, 7) == 1);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_3))(0, 1, 2, 3, 4, 5, 6, 7) == 2);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_4))(0, 1, 2, 3, 4, 5, 6, 7) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_5))(0, 1, 2, 3, 4, 5, 6, 7) == 4);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_6))(0, 1, 2, 3, 4, 5, 6, 7) == 5);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_7))(0, 1, 2, 3, 4, 5, 6, 7) == 6);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_8))(0, 1, 2, 3, 4, 5, 6, 7) == 7);

    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 0);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 1);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_3))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 2);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_4))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_5))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 4);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_6))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 5);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_7))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 6);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_8))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 7);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_9))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 8);

    // test mixed perfect forwarding
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1))(i[0], 1) == 0);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2))(i[0], 1) == 1);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1))(0, i[1]) == 0);
    BOOST_FIT_TEST_CHECK(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2))(0, i[1]) == 1);

    // const

    // test nullary
    BOOST_FIT_TEST_CHECK(constify(constify(boost::fit::protect(boost::fit::lazy(f)(1))))() == 1);

    // test lvalues
    BOOST_FIT_TEST_CHECK(&constify(constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1))))(i[0]) == &i[0]);

    BOOST_FIT_TEST_CHECK(&constify(constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1))))(i[0], i[1]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&constify(constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2))))(i[0], i[1]) == &i[1]);

    BOOST_FIT_TEST_CHECK(&constify(constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1))))(i[0], i[1], i[2]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&constify(constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2))))(i[0], i[1], i[2]) == &i[1]);
    BOOST_FIT_TEST_CHECK(&constify(constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_3))))(i[0], i[1], i[2]) == &i[2]);

    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1)))(i[0], i[1], i[2], i[3]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2)))(i[0], i[1], i[2], i[3]) == &i[1]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_3)))(i[0], i[1], i[2], i[3]) == &i[2]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_4)))(i[0], i[1], i[2], i[3]) == &i[3]);

    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1)))(i[0], i[1], i[2], i[3], i[4]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2)))(i[0], i[1], i[2], i[3], i[4]) == &i[1]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_3)))(i[0], i[1], i[2], i[3], i[4]) == &i[2]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_4)))(i[0], i[1], i[2], i[3], i[4]) == &i[3]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_5)))(i[0], i[1], i[2], i[3], i[4]) == &i[4]);

    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1)))(i[0], i[1], i[2], i[3], i[4], i[5]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2)))(i[0], i[1], i[2], i[3], i[4], i[5]) == &i[1]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_3)))(i[0], i[1], i[2], i[3], i[4], i[5]) == &i[2]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_4)))(i[0], i[1], i[2], i[3], i[4], i[5]) == &i[3]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_5)))(i[0], i[1], i[2], i[3], i[4], i[5]) == &i[4]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_6)))(i[0], i[1], i[2], i[3], i[4], i[5]) == &i[5]);

    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6]) == &i[1]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_3)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6]) == &i[2]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_4)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6]) == &i[3]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_5)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6]) == &i[4]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_6)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6]) == &i[5]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_7)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6]) == &i[6]);

    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[1]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_3)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[2]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_4)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[3]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_5)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[4]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_6)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[5]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_7)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[6]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_8)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7]) == &i[7]);

    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_1)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[0]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_2)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[1]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_3)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[2]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_4)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[3]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_5)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[4]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_6)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[5]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_7)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[6]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_8)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[7]);
    BOOST_FIT_TEST_CHECK(&constify(boost::fit::protect(boost::fit::lazy(g)(std::placeholders::_9)))(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8]) == &i[8]);

    // test rvalues

    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1)))(0) == 0);

    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1)))(0, 1) == 0);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2)))(0, 1) == 1);

    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1)))(0, 1, 2) == 0);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2)))(0, 1, 2) == 1);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_3)))(0, 1, 2) == 2);

    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1)))(0, 1, 2, 3) == 0);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2)))(0, 1, 2, 3) == 1);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_3)))(0, 1, 2, 3) == 2);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_4)))(0, 1, 2, 3) == 3);

    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1)))(0, 1, 2, 3, 4) == 0);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2)))(0, 1, 2, 3, 4) == 1);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_3)))(0, 1, 2, 3, 4) == 2);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_4)))(0, 1, 2, 3, 4) == 3);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_5)))(0, 1, 2, 3, 4) == 4);

    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1)))(0, 1, 2, 3, 4, 5) == 0);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2)))(0, 1, 2, 3, 4, 5) == 1);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_3)))(0, 1, 2, 3, 4, 5) == 2);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_4)))(0, 1, 2, 3, 4, 5) == 3);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_5)))(0, 1, 2, 3, 4, 5) == 4);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_6)))(0, 1, 2, 3, 4, 5) == 5);

    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1)))(0, 1, 2, 3, 4, 5, 6) == 0);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2)))(0, 1, 2, 3, 4, 5, 6) == 1);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_3)))(0, 1, 2, 3, 4, 5, 6) == 2);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_4)))(0, 1, 2, 3, 4, 5, 6) == 3);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_5)))(0, 1, 2, 3, 4, 5, 6) == 4);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_6)))(0, 1, 2, 3, 4, 5, 6) == 5);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_7)))(0, 1, 2, 3, 4, 5, 6) == 6);

    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1)))(0, 1, 2, 3, 4, 5, 6, 7) == 0);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2)))(0, 1, 2, 3, 4, 5, 6, 7) == 1);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_3)))(0, 1, 2, 3, 4, 5, 6, 7) == 2);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_4)))(0, 1, 2, 3, 4, 5, 6, 7) == 3);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_5)))(0, 1, 2, 3, 4, 5, 6, 7) == 4);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_6)))(0, 1, 2, 3, 4, 5, 6, 7) == 5);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_7)))(0, 1, 2, 3, 4, 5, 6, 7) == 6);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_8)))(0, 1, 2, 3, 4, 5, 6, 7) == 7);

    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1)))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 0);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2)))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 1);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_3)))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 2);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_4)))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 3);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_5)))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 4);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_6)))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 5);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_7)))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 6);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_8)))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 7);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_9)))(0, 1, 2, 3, 4, 5, 6, 7, 8) == 8);

    // test mixed perfect forwarding
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1)))(i[0], 1) == 0);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2)))(i[0], 1) == 1);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_1)))(0, i[1]) == 0);
    BOOST_FIT_TEST_CHECK(constify(boost::fit::protect(boost::fit::lazy(f)(std::placeholders::_2)))(0, i[1]) == 1);
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::lazy(boost::fit::apply)(boost::fit::protect(boost::fit::lazy(boost::fit::identity)(boost::fit::_1)), boost::fit::_1)(17) == 17);
    BOOST_FIT_TEST_CHECK(boost::fit::lazy(boost::fit::apply)(boost::fit::protect(boost::fit::lazy(boost::fit::identity)(boost::fit::_1)), 17)() == 17);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::lazy(boost::fit::apply)(boost::fit::protect(boost::fit::lazy(boost::fit::identity)(boost::fit::_1)), boost::fit::_1)(17) == 17);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::lazy(boost::fit::apply)(boost::fit::protect(boost::fit::lazy(boost::fit::identity)(boost::fit::_1)), 17)() == 17);
}

namespace test1 {

int id(int x)
{
    return x;
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::lazy(boost::fit::apply)(boost::fit::protect(boost::fit::lazy(id)(std::placeholders::_1)), std::placeholders::_1)(17) == 17);
    BOOST_FIT_TEST_CHECK(boost::fit::lazy(boost::fit::apply)(boost::fit::protect(boost::fit::lazy(id)(std::placeholders::_1)), 17)() == 17);
}

}
