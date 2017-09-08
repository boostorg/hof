#include <boost/fit/decorate.hpp>
#include "test.hpp"

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::decorate(boost::fit::always(1))(boost::fit::always(1))(boost::fit::always(1))(5) == 1);
}

#if BOOST_FIT_HAS_NOEXCEPT_DEDUCTION
struct copy_throws 
{
    copy_throws() {}
    copy_throws(copy_throws const&) {}
    copy_throws(copy_throws&&) noexcept {}
};

struct no_throw_fo 
{
    void operator()() const noexcept {}
    void operator()(copy_throws) const noexcept {}
};

BOOST_FIT_TEST_CASE()
{
    static_assert(noexcept(boost::fit::decorate(boost::fit::always(1))(boost::fit::always(1))(boost::fit::always(1))(5)), "noexcept decorator");
    static_assert(!noexcept(boost::fit::decorate(boost::fit::always(1))(boost::fit::always(1))(boost::fit::always(1))(copy_throws{})), "noexcept decorator");
}

#endif
