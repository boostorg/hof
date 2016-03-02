#include <boost/fit/mutable.hpp>
#include <boost/fit/lazy.hpp>
#include <boost/fit/detail/move.hpp>
#include <memory>
#include "test.hpp"

struct mutable_fun
{
    int x;
    mutable_fun() : x(1)
    {}

    int operator()(int i)
    {
        x+=i;
        return x;
    }
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::mutable_(mutable_fun())(3) == 4);
}

BOOST_FIT_TEST_CASE()
{
    auto mut_fun = mutable_fun();
    auto by_5 = boost::fit::lazy(boost::fit::mutable_(std::ref(mut_fun)))(5);
    BOOST_FIT_TEST_CHECK(by_5() == 6);
    BOOST_FIT_TEST_CHECK(by_5() == 11);
}

struct mutable_move_fun
{
    std::unique_ptr<int> x;
    mutable_move_fun() : x(new int(1))
    {}

    int operator()(int i)
    {
        *x+=i;
        return *x;
    }
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::mutable_(mutable_move_fun())(3) == 4);
}

BOOST_FIT_TEST_CASE()
{
    auto mut_fun = mutable_move_fun();
    auto by_5 = boost::fit::lazy(boost::fit::mutable_(boost::fit::move(mut_fun)))(5);
    BOOST_FIT_TEST_CHECK(by_5() == 6);
    BOOST_FIT_TEST_CHECK(by_5() == 11);
}

