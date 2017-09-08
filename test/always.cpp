#include <boost/fit/always.hpp>
#include <boost/fit/function.hpp>
#include <memory>
#include "test.hpp"

BOOST_FIT_TEST_CASE()
{
    static const int ten = 10;
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::always(ten)(1,2,3,4,5) == 10);
    BOOST_FIT_TEST_CHECK( boost::fit::always(ten)(1,2,3,4,5) == 10 );
    
    int i = 10; 
    BOOST_FIT_TEST_CHECK( boost::fit::always(std::ref(i))(1,2,3,4,5) == 10 );
    BOOST_FIT_TEST_CHECK( &boost::fit::always(std::ref(i))(1,2,3,4,5) == &i );

    boost::fit::always()(1, 2);
    static_assert(std::is_same<decltype(boost::fit::always()(1, 2)), BOOST_FIT_ALWAYS_VOID_RETURN>::value, "Failed");
}

BOOST_FIT_TEST_CASE()
{
    int i = 10; 
    BOOST_FIT_TEST_CHECK( boost::fit::always_ref(i)(1,2,3,4,5) == 10 );
    BOOST_FIT_TEST_CHECK( &boost::fit::always_ref(i)(1,2,3,4,5) == &i );
}

BOOST_FIT_STATIC_FUNCTION(gten) = boost::fit::always(std::integral_constant<int, 10>{});

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(gten(1,2,3,4,5) == 10);
    BOOST_FIT_TEST_CHECK(gten(1,2,3,4,5) == 10);
}

BOOST_FIT_TEST_CASE()
{
    auto f = boost::fit::always(10);
    STATIC_ASSERT_NOT_DEFAULT_CONSTRUCTIBLE(decltype(f));
    BOOST_FIT_TEST_CHECK(f(1,2,3,4,5) == 10);
}

struct copy_throws 
{
    copy_throws() {}
    copy_throws(copy_throws const&) {}
    copy_throws(copy_throws&&) noexcept {}
};

BOOST_FIT_TEST_CASE()
{
    static_assert(noexcept(boost::fit::always()()), "noexcept always");
    static_assert(noexcept(boost::fit::always(1)()), "noexcept always");
    static_assert(!noexcept(boost::fit::always(copy_throws{})()), "noexcept always");
    copy_throws ct{};
    static_assert(!noexcept(boost::fit::always(ct)()), "noexcept always");
    static_assert(noexcept(boost::fit::always(std::ref(ct))()) == BOOST_FIT_IS_NOTHROW_COPY_CONSTRUCTIBLE(std::reference_wrapper<copy_throws>), "noexcept always");
    auto ctf = boost::fit::always(copy_throws{});
    static_assert(!noexcept(ctf()), "noexcept always");
}
