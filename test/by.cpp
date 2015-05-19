#include <fit/by.h>
#include <fit/placeholders.h>
#include <fit/mutable.h>
#include "test.h"

#include <memory>

struct foo
{
    constexpr foo(int x) : x(x)
    {}
    int x;
};

struct select_x
{
    template<class T>
    constexpr auto operator()(T&& x) const FIT_RETURNS(x.x);
};

FIT_TEST_CASE()
{
    constexpr auto add = fit::_ + fit::_;
    FIT_STATIC_TEST_CHECK(fit::by(select_x(), add)(foo(1), foo(2)) == 3);
    // Using mutable_ as a workaround on libc++, since mem_fn does not meet the
    // requirements of a FunctionObject
    FIT_TEST_CHECK(fit::by(fit::mutable_(std::mem_fn(&foo::x)), add)(foo(1), foo(2)) == 3);
}

struct select_x_1
{
    std::unique_ptr<int> i;
    select_x_1() : i(new int(2))
    {}
    template<class T>
    auto operator()(T&& x) const FIT_RETURNS(x.x * (*i));
};

struct sum_1
{
    std::unique_ptr<int> i;
    sum_1() : i(new int(1))
    {}
    template<class T, class U>
    auto operator()(T&& x, U&& y) const FIT_RETURNS(x + y + *i);
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::by(select_x_1(), sum_1())(foo(1), foo(2)) == 7);
}

FIT_TEST_CASE()
{
    std::string s;
    auto f = [&](const std::string& x)
    {
        s += x;
    };
    fit::by(f)("hello", "-", "world");
    FIT_TEST_CHECK(s == "hello-world");
    fit::by(f)();
    FIT_TEST_CHECK(s == "hello-world");
}

FIT_TEST_CASE()
{
    std::string s;
    auto f = [&](const std::string& x)
    {
        s += x;
        return s;
    };
    auto last = [](const std::string& x, const std::string& y, const std::string& z)
    {
        FIT_TEST_CHECK(x == "hello");
        FIT_TEST_CHECK(y == "hello-");
        FIT_TEST_CHECK(z == "hello-world");
        return z;
    };
    FIT_TEST_CHECK(fit::by(f, last)("hello", "-", "world") == "hello-world");
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
        return fit::by(constexpr_check())(x, x), true;
    }
};

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(constexpr_check_each()(bool_<true>()));
}
