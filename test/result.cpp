#include <boost/fit/result.hpp>
#include <boost/fit/static.hpp>
#include "test.hpp"


static constexpr boost::fit::result_adaptor<int, unary_class> unary_int = {};

BOOST_FIT_TEST_CASE()
{
    STATIC_ASSERT_SAME(decltype(unary_int(false)), int);
    BOOST_FIT_TEST_CHECK(unary_int(false) == 0);
    BOOST_FIT_STATIC_TEST_CHECK(unary_int(false) == 0);
}

static constexpr boost::fit::result_adaptor<void, unary_class> unary_void = {};

BOOST_FIT_TEST_CASE()
{
    STATIC_ASSERT_SAME(decltype(unary_void(false)), void);
}
