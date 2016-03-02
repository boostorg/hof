#include <boost/fit/match.hpp>
#include <boost/fit/static.hpp>
#include <boost/fit/lambda.hpp>
#include "test.hpp"

#include <memory>

namespace test1
{
    struct int_class
    {
        int operator()(int) const
        {
            return 1;
        }
    };

    struct foo
    {};

    struct foo_class
    {
        foo operator()(foo) const
        {
            return foo();
        }
    };

    static constexpr boost::fit::static_<boost::fit::match_adaptor<int_class, foo_class> > fun = {};

    static_assert(std::is_same<int, decltype(fun(1))>::value, "Failed match");
    static_assert(std::is_same<foo, decltype(fun(foo()))>::value, "Failed match");
}

struct int_class
{
    constexpr int operator()(int) const
    {
        return 1;
    }
};

struct foo
{};

struct foo_class
{
    constexpr int operator()(foo) const
    {
        return 2;
    }
};

static constexpr boost::fit::match_adaptor<int_class, foo_class> fun = {};

BOOST_FIT_TEST_CASE()
{
    
    BOOST_FIT_TEST_CHECK(fun(1) == 1);
    BOOST_FIT_TEST_CHECK(fun(foo()) == 2);

    BOOST_FIT_STATIC_TEST_CHECK(fun(1) == 1);
    BOOST_FIT_STATIC_TEST_CHECK(fun(foo()) == 2);
};

BOOST_FIT_TEST_CASE()
{
    
    BOOST_FIT_TEST_CHECK(boost::fit::reveal(fun)(1) == 1);
    BOOST_FIT_TEST_CHECK(boost::fit::reveal(fun)(foo()) == 2);

    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::reveal(fun)(1) == 1);
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::reveal(fun)(foo()) == 2);
};
#if BOOST_FIT_HAS_STATIC_LAMBDA
BOOST_FIT_TEST_CASE()
{
    
    constexpr auto lam = boost::fit::match(
        BOOST_FIT_STATIC_LAMBDA(int) { return 1; },
        BOOST_FIT_STATIC_LAMBDA(foo) { return 2; }
    );
    
    BOOST_FIT_TEST_CHECK(lam(1) == 1);
    BOOST_FIT_TEST_CHECK(lam(foo()) == 2);
};
#endif

BOOST_FIT_TEST_CASE()
{
    int i = 0;
    auto lam = boost::fit::match(
        [&](int) { return i+1; },
        [&](foo) { return i+2; }
    );
// Disable this check on msvc, since lambdas might be default constructible
#ifndef _MSC_VER
    STATIC_ASSERT_NOT_DEFAULT_CONSTRUCTIBLE(decltype(lam));
#endif
    BOOST_FIT_TEST_CHECK(lam(1) == 1);
    BOOST_FIT_TEST_CHECK(lam(foo()) == 2);
};


BOOST_FIT_TEST_CASE()
{
    struct not_default_constructible
    {
        int i;
        not_default_constructible(int x) : i(x)
        {}
    };
    STATIC_ASSERT_NOT_DEFAULT_CONSTRUCTIBLE(not_default_constructible);
    not_default_constructible ndc = not_default_constructible(0);
    auto lam = boost::fit::match(
        [&](int) { return ndc.i+1; },
        [&](foo) { return ndc.i+2; }
    );
// Disable this check on msvc, since lambdas might be default constructible
#ifndef _MSC_VER
    STATIC_ASSERT_NOT_DEFAULT_CONSTRUCTIBLE(decltype(lam));
#endif
    
    BOOST_FIT_TEST_CHECK(lam(1) == 1);
    BOOST_FIT_TEST_CHECK(lam(foo()) == 2);
};


struct int_move_class
{
    std::unique_ptr<int> i;
    int_move_class() : i(new int(1))
    {}
    int operator()(int) const
    {
        return *i;
    }
};

struct foo_move_class
{
    std::unique_ptr<int> i;
    foo_move_class() : i(new int(2))
    {}
    int operator()(foo) const
    {
        return *i;
    }
};

BOOST_FIT_TEST_CASE()
{
    auto fun_move = boost::fit::match(int_move_class(), foo_move_class());
    BOOST_FIT_TEST_CHECK(fun_move(1) == 1);
    BOOST_FIT_TEST_CHECK(fun_move(foo()) == 2);

};


