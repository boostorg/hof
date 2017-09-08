#include <boost/fit/indirect.hpp>
#include "test.hpp"

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == boost::fit::indirect(std::unique_ptr<binary_class>(new binary_class()))(1, 2));
    BOOST_FIT_TEST_CHECK(3 == boost::fit::reveal(boost::fit::indirect(std::unique_ptr<binary_class>(new binary_class())))(1, 2));

    binary_class f;

    BOOST_FIT_TEST_CHECK(3 == boost::fit::indirect(&f)(1, 2));
    BOOST_FIT_TEST_CHECK(3 == boost::fit::reveal(boost::fit::indirect(&f))(1, 2));
}
#if BOOST_FIT_HAS_NOEXCEPT_DEDUCTION
BOOST_FIT_TEST_CASE()
{
    binary_class f;
    static_assert(noexcept(boost::fit::indirect(&f)(1, 2)), "noexcept indirect");
}
#endif

struct mutable_function
{
    mutable_function() : value(0) {}
    void operator()(int a) { value += a; }
    int value;
};

BOOST_FIT_TEST_CASE()
{
    auto mf = mutable_function{};
    boost::fit::indirect(&mf)(15);
    boost::fit::indirect(&mf)(2);
    BOOST_FIT_TEST_CHECK(mf.value == 17);
}


BOOST_FIT_TEST_CASE()
{
    auto mf = std::make_shared<mutable_function>();
    boost::fit::indirect(mf)(15);
    boost::fit::indirect(mf)(2);
    BOOST_FIT_TEST_CHECK(mf->value == 17);
}


