#include <boost/fit/capture.hpp>
#include <boost/fit/identity.hpp>
#include "test.hpp"

BOOST_FIT_TEST_CASE()
{    
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::capture_basic(1, 2)(binary_class())() == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture_basic(1, 2)(binary_class())() == 3);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::capture_basic(1)(binary_class())(2) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture_basic(1)(binary_class())(2) == 3);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::capture_basic()(binary_class())(1, 2) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture_basic()(binary_class())(1, 2) == 3);

    static const int one = 1;
    static const int two = 2;
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::capture_forward(one, two)(binary_class())() == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture_forward(one, two)(binary_class())() == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture_forward(1, 2)(binary_class())() == 3);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::capture_forward(one)(binary_class())(two) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture_forward(1)(binary_class())(2) == 3);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::capture_forward()(binary_class())(one, two) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture_forward()(binary_class())(one, two) == 3);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::capture(1, 2)(binary_class())() == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture(1, 2)(binary_class())() == 3);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::capture(1)(binary_class())(2) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture(1)(binary_class())(2) == 3);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::capture()(binary_class())(1, 2) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture()(binary_class())(1, 2) == 3);
}

struct add_member
{
    int i;

    add_member(int ip) : i(ip)
    {}

    int add(int j) const
    {
        return i + j;
    }
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::capture_basic(add_member(1), 2)(&add_member::add)() == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::capture_basic(add_member(1))(&add_member::add)(2) == 3);
}

BOOST_FIT_TEST_CASE()
{
    auto id = boost::fit::identity;
    auto f = boost::fit::capture(boost::fit::identity)(boost::fit::identity);
    static_assert(BOOST_FIT_IS_DEFAULT_CONSTRUCTIBLE(decltype(id)), "Id not default constructible");
    static_assert(BOOST_FIT_IS_DEFAULT_CONSTRUCTIBLE(decltype(f)), "Not default constructible");
    f();
}
#if BOOST_FIT_HAS_NOEXCEPT_DEDUCTION
BOOST_FIT_TEST_CASE()
{
    static_assert(noexcept(boost::fit::capture(boost::fit::identity)(boost::fit::identity)()), "noexcept capture");
    static_assert(noexcept(boost::fit::capture_basic(boost::fit::identity)(boost::fit::identity)()), "noexcept capture");
    static_assert(noexcept(boost::fit::capture_forward(boost::fit::identity)(boost::fit::identity)()), "noexcept capture");
}
#endif
BOOST_FIT_TEST_CASE()
{
    auto f = boost::fit::capture_basic(boost::fit::identity)(boost::fit::identity);
    f();
}

BOOST_FIT_TEST_CASE()
{
    auto f = boost::fit::capture_forward(boost::fit::identity)(boost::fit::identity);
    f();
}

BOOST_FIT_TEST_CASE()
{
    auto f = boost::fit::capture(boost::fit::identity)(add_member{1});
    static_assert(!boost::fit::is_callable<decltype(f), int>::value, "Not sfinae friendly");
}

