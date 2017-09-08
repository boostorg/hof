#include <boost/fit/identity.hpp>
#include <boost/fit/is_callable.hpp>
#include <boost/fit/detail/move.hpp>
#include "test.hpp"

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::identity(10) == 10);
    BOOST_FIT_TEST_CHECK(boost::fit::identity(10) == 10);
}

BOOST_FIT_TEST_CASE()
{
    int i = 3;
    BOOST_FIT_TEST_CHECK(boost::fit::identity(i) == 3);
    BOOST_FIT_TEST_CHECK(&boost::fit::identity(i) == &i);
    static_assert(std::is_lvalue_reference<decltype(boost::fit::identity(i))>::value, "Not lvalue");
    static_assert(!std::is_lvalue_reference<decltype(boost::fit::identity(3))>::value, "Not rvalue");
}

BOOST_FIT_TEST_CASE()
{
    auto ls = boost::fit::identity({1, 2, 3, 4});
    std::vector<int> v{1, 2, 3, 4};
    BOOST_FIT_TEST_CHECK(std::equal(ls.begin(), ls.end(), v.begin()));
}

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(std::vector<int>(boost::fit::identity({1, 2, 3})) == std::vector<int>{1, 2, 3});
}

BOOST_FIT_TEST_CASE()
{
    static_assert(boost::fit::is_callable<decltype(boost::fit::identity), int>::value, "Identiy callable");
    static_assert(!boost::fit::is_callable<decltype(boost::fit::identity), int, int>::value, "Identiy not callable");
    static_assert(!boost::fit::is_callable<decltype(boost::fit::identity)>::value, "Identiy not callable");
}

BOOST_FIT_TEST_CASE()
{
    static_assert(noexcept(boost::fit::identity({1, 2, 3})), "Noexcept identity");
    static_assert(noexcept(boost::fit::identity(1)), "Noexcept identity");
    int i = 5;
    static_assert(noexcept(boost::fit::identity(i)), "Noexcept identity");
}

struct copy_throws 
{
    copy_throws() {}
    copy_throws(copy_throws const&) {}
    copy_throws(copy_throws&&) noexcept {}
};

BOOST_FIT_TEST_CASE()
{
    copy_throws ct{};
    static_assert(noexcept(boost::fit::identity(ct)), "Noexcept identity");
    static_assert(noexcept(boost::fit::identity(boost::fit::move(ct))), "Noexcept identity");
    static_assert(!noexcept(boost::fit::identity(copy_throws{})), "Noexcept identity");
}

