#include <boost/fit/pipable.hpp>
#include <boost/fit/static.hpp>
#include "test.hpp"

static constexpr boost::fit::static_<boost::fit::pipable_adaptor<binary_class> > binary_pipable = {};

static constexpr boost::fit::static_<boost::fit::pipable_adaptor<unary_class> > unary_pipable = {};

static constexpr boost::fit::static_<boost::fit::pipable_adaptor<mutable_class> > mutable_pipable = {};

static constexpr boost::fit::static_<boost::fit::pipable_adaptor<void_class> > void_pipable = {};

static constexpr boost::fit::static_<boost::fit::pipable_adaptor<mono_class> > mono_pipable = {};

static constexpr boost::fit::static_<boost::fit::pipable_adaptor<move_class> > move_pipable = {};

constexpr boost::fit::pipable_adaptor<void_class> void_pipable_constexpr = {};

constexpr boost::fit::pipable_adaptor<binary_class> binary_pipable_constexpr = {};

constexpr boost::fit::pipable_adaptor<unary_class> unary_pipable_constexpr = {};


BOOST_FIT_TEST_CASE()
{
    void_pipable(1);
    1 | void_pipable;
    BOOST_FIT_TEST_CHECK(3 == (1 | binary_pipable(2)));
    BOOST_FIT_TEST_CHECK(3 == (binary_pipable(1, 2)));
    BOOST_FIT_TEST_CHECK(3 == (3 | unary_pipable));
    BOOST_FIT_TEST_CHECK(3 == (3 | unary_pipable()));
    BOOST_FIT_TEST_CHECK(3 == (unary_pipable(3)));
    int i1 = 1;
    BOOST_FIT_TEST_CHECK(3 == (2 | binary_pipable(i1)));
    BOOST_FIT_TEST_CHECK(3 == (i1 | mutable_pipable(2)));
    BOOST_FIT_TEST_CHECK(3 == (i1));
    int i2 = 1;
    BOOST_FIT_TEST_CHECK(3 == (mutable_pipable(i2, 2)));
    BOOST_FIT_TEST_CHECK(3 == (i2));
    BOOST_FIT_TEST_CHECK(3 == (mono_pipable(2)));
    BOOST_FIT_TEST_CHECK(3 == (2| mono_pipable));
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == (move_class()(1, 2)));
    BOOST_FIT_TEST_CHECK(3 == (move_pipable(1, 2)));
    BOOST_FIT_TEST_CHECK(3 == (1 | move_pipable(2)));
    BOOST_FIT_TEST_CHECK(3 == (1 | boost::fit::pipable(move_class())(2)));
    BOOST_FIT_TEST_CHECK(3 == (boost::fit::pipable(move_class())(1, 2)));
}

BOOST_FIT_TEST_CASE()
{
    void_pipable_constexpr(1);
    1 | void_pipable_constexpr;
    BOOST_FIT_STATIC_TEST_CHECK(3 == (1 | binary_pipable_constexpr(2)));
    BOOST_FIT_STATIC_TEST_CHECK(3 == (binary_pipable_constexpr(1, 2)));
    BOOST_FIT_STATIC_TEST_CHECK(3 == (3 | unary_pipable_constexpr));
    BOOST_FIT_STATIC_TEST_CHECK(3 == (3 | unary_pipable_constexpr()));
    BOOST_FIT_STATIC_TEST_CHECK(3 == (unary_pipable_constexpr(3)));
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == boost::fit::limit_c<2>(binary_pipable_constexpr)(1, 2));
    BOOST_FIT_STATIC_TEST_CHECK(3 == boost::fit::limit_c<2>(binary_pipable_constexpr)(1, 2));
}

BOOST_FIT_TEST_CASE()
{
    auto f = boost::fit::pipable(boost::fit::limit_c<2>(binary_class()));
    static_assert(boost::fit::is_callable<decltype(f), int, int>::value, "Passing the limit is not callable");
    static_assert(!boost::fit::is_callable<decltype(f), int, int, int>::value, "Passing the limit is not callable");
    static_assert(!boost::fit::is_callable<decltype(f), int, int, int, int>::value, "Passing the limit is not callable");
}
