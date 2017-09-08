#include <boost/fit/arg.hpp>
#include <boost/fit/is_callable.hpp>
#include <type_traits>
#include "test.hpp"

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::arg_c<3>(1,2,3,4,5) == 3);
    BOOST_FIT_TEST_CHECK( boost::fit::arg_c<3>(1,2,3,4,5) == 3 );
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::arg(std::integral_constant<int, 3>())(1,2,3,4,5) == 3);
    BOOST_FIT_TEST_CHECK( boost::fit::arg(std::integral_constant<int, 3>())(1,2,3,4,5) == 3 );
}

BOOST_FIT_TEST_CASE()
{
    auto at_3 = boost::fit::arg(std::integral_constant<int, 3>());
    static_assert(boost::fit::is_callable<decltype(at_3), int, int, int>::value, "Not SFINAE-friendly");
    static_assert(!boost::fit::is_callable<decltype(at_3), int, int>::value, "Not SFINAE-friendly");
    static_assert(!boost::fit::is_callable<decltype(at_3), int>::value, "Not SFINAE-friendly");
}

struct foo {};

BOOST_FIT_TEST_CASE()
{
    static_assert(!boost::fit::is_callable<decltype(boost::fit::arg), int>::value, "Not sfinae friendly");
    static_assert(!boost::fit::is_callable<decltype(boost::fit::arg), foo>::value, "Not sfinae friendly");
}

struct copy_throws 
{
    copy_throws() {}
    copy_throws(copy_throws const&) {}
    copy_throws(copy_throws&&) noexcept {}
};
#if BOOST_FIT_HAS_NOEXCEPT_DEDUCTION
BOOST_FIT_TEST_CASE()
{
    static_assert(noexcept(boost::fit::arg_c<3>(1,2,3,4,5)), "noexcept arg");
    static_assert(noexcept(boost::fit::arg(std::integral_constant<int, 3>())(1,2,3,4,5)), "noexcept arg");
    static_assert(!noexcept(boost::fit::arg(std::integral_constant<int, 3>())(1,2,copy_throws{},4,5)), "noexcept arg");
}
#endif
