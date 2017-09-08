#include <boost/fit/fix.hpp>
#include <boost/fit/static.hpp>
#include <boost/fit/reveal.hpp>
#include <boost/fit/result.hpp>
#include "test.hpp"

#include <memory>

struct factorial_t
{
    template<class Self, class T>
    T operator()(Self s, T x) const noexcept
    {
        return x == 0 ? 1 : x * s(x-1);
    }
};

struct factorial_constexpr_t
{
    template<class Self, class T>
    constexpr T operator()(Self s, T x) const noexcept
    {
        return x == 0 ? 1 : x * s(x-1);
    }
};

struct factorial_move_t
{
    std::unique_ptr<int> i;
    factorial_move_t() : i(new int(1))
    {}
    template<class Self, class T>
    T operator()(const Self& s, T x) const
    {
        return x == 0 ? *i : x * s(x-1);
    }
};

static constexpr boost::fit::fix_adaptor<factorial_t> factorial = {};
static constexpr boost::fit::fix_adaptor<factorial_constexpr_t> factorial_constexpr = {};
static constexpr boost::fit::static_<boost::fit::fix_adaptor<factorial_move_t> > factorial_move = {};

#if BOOST_FIT_HAS_NOEXCEPT_DEDUCTION
BOOST_FIT_TEST_CASE()
{
    static_assert(noexcept(factorial(5)), "noexcept fix");
}
#endif

BOOST_FIT_TEST_CASE()
{
    const int r = factorial(5);
    BOOST_FIT_TEST_CHECK(r == 5*4*3*2*1);
}

BOOST_FIT_TEST_CASE()
{
    const int r = boost::fit::reveal(factorial)(5);
    BOOST_FIT_TEST_CHECK(r == 5*4*3*2*1);
}

#if !BOOST_FIT_NO_EXPRESSION_SFINAE
BOOST_FIT_TEST_CASE()
{
    const int r = boost::fit::fix(boost::fit::result<int>(factorial_constexpr_t()))(5);
    BOOST_FIT_TEST_CHECK(r == 5*4*3*2*1);
}

BOOST_FIT_TEST_CASE()
{
    const int r = boost::fit::result<int>(factorial_constexpr)(5);
    BOOST_FIT_TEST_CHECK(r == 5*4*3*2*1);
}
BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::fix(boost::fit::result<int>(factorial_constexpr_t()))(5) == 5*4*3*2*1);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::result<int>(factorial_constexpr)(5) == 5*4*3*2*1);
}
#endif
BOOST_FIT_TEST_CASE()
{
#if BOOST_FIT_HAS_GENERIC_LAMBDA
    auto factorial_ = boost::fit::fix([](auto s, auto x) -> decltype(x) { return x == 0 ? 1 : x * s(x-1); });
    int r = boost::fit::result<int>(factorial_)(5);
    BOOST_FIT_TEST_CHECK(r == 5*4*3*2*1);
#endif
}

BOOST_FIT_TEST_CASE()
{
    const int r = factorial_move(5);
    BOOST_FIT_TEST_CHECK(r == 5*4*3*2*1);
    BOOST_FIT_TEST_CHECK(boost::fit::fix(factorial_move_t())(5) == 5*4*3*2*1);
}
