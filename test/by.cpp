#include <boost/fit/by.hpp>
#include <boost/fit/placeholders.hpp>
#include <boost/fit/mutable.hpp>
#include "test.hpp"

#include <memory>

struct foo
{
    constexpr foo(int xp) : x(xp)
    {}
    int x;
};

struct select_x
{
    template<class T>
    constexpr auto operator()(T&& x) const BOOST_FIT_RETURNS(x.x);
};

BOOST_FIT_TEST_CASE()
{
#ifndef _MSC_VER
    constexpr 
#endif
    auto add = boost::fit::_ + boost::fit::_;
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::by(select_x(), add)(foo(1), foo(2)) == 3);
    // Using mutable_ as a workaround on libc++, since mem_fn does not meet the
    // requirements of a FunctionObject
    BOOST_FIT_TEST_CHECK(boost::fit::by(boost::fit::mutable_(std::mem_fn(&foo::x)), add)(foo(1), foo(2)) == 3);
    static_assert(boost::fit::detail::is_default_constructible<decltype(boost::fit::by(select_x(), add))>::value, "Not default constructible");
}

BOOST_FIT_TEST_CASE()
{
#ifndef _MSC_VER
    constexpr 
#endif
    auto add = boost::fit::_ + boost::fit::_;
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::by(select_x(), add)(foo(1), foo(2)) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::by(&foo::x, add)(foo(1), foo(2)) == 3);
    static_assert(boost::fit::detail::is_default_constructible<decltype(boost::fit::by(select_x(), add))>::value, "Not default constructible");
}

BOOST_FIT_TEST_CASE()
{
    auto indirect_add = boost::fit::by(*boost::fit::_, boost::fit::_ + boost::fit::_);
    BOOST_FIT_TEST_CHECK(indirect_add(std::unique_ptr<int>(new int(1)), std::unique_ptr<int>(new int(2))) == 3);
    static_assert(boost::fit::detail::is_default_constructible<decltype(indirect_add)>::value, "Not default constructible");
}

struct select_x_1
{
    std::unique_ptr<int> i;
    select_x_1() : i(new int(2))
    {}
    template<class T>
    auto operator()(T&& x) const BOOST_FIT_RETURNS(x.x * (*i));
};

struct sum_1
{
    std::unique_ptr<int> i;
    sum_1() : i(new int(1))
    {}
    template<class T, class U>
    auto operator()(T&& x, U&& y) const BOOST_FIT_RETURNS(x + y + *i);
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::by(select_x_1(), sum_1())(foo(1), foo(2)) == 7);
}

BOOST_FIT_TEST_CASE()
{
    std::string s;
    auto f = [&](const std::string& x)
    {
        s += x;
    };
    boost::fit::by(f)("hello", "-", "world");
    BOOST_FIT_TEST_CHECK(s == "hello-world");
    boost::fit::by(f)();
    BOOST_FIT_TEST_CHECK(s == "hello-world");
}

BOOST_FIT_TEST_CASE()
{
    std::string s;
    auto f = [&](const std::string& x)
    {
        s += x;
        return s;
    };
    auto last = [](const std::string& x, const std::string& y, const std::string& z)
    {
        BOOST_FIT_TEST_CHECK(x == "hello");
        BOOST_FIT_TEST_CHECK(y == "hello-");
        BOOST_FIT_TEST_CHECK(z == "hello-world");
        return z;
    };
    BOOST_FIT_TEST_CHECK(boost::fit::by(f, last)("hello", "-", "world") == "hello-world");
}

template<bool B>
struct bool_
{
    static const bool value = B;
};

struct constexpr_check
{
    template<class T>
    constexpr int operator()(T) const
    {
        static_assert(T::value, "Failed");
        return 0;
    }
};

struct constexpr_check_each
{
    template<class T>
    constexpr bool operator()(T x) const
    {
        return boost::fit::by(constexpr_check())(x, x), true;
    }
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_STATIC_TEST_CHECK(constexpr_check_each()(bool_<true>()));
}
