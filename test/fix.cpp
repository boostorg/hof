#include <fit/fix.hpp>
#include <fit/static.hpp>
#include <fit/reveal.hpp>
#include <fit/result.hpp>
#include "test.hpp"

#include <memory>

struct factorial_t
{
    template<class Self, class T>
    T operator()(Self s, T x) const
    {
        return x == 0 ? 1 : x * s(x-1);
    }
};

struct factorial_constexpr_t
{
    template<class Self, class T>
    constexpr T operator()(Self s, T x) const
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

static constexpr fit::fix_adaptor<factorial_t> factorial = {};
static constexpr fit::fix_adaptor<factorial_constexpr_t> factorial_constexpr = {};
static constexpr fit::static_<fit::fix_adaptor<factorial_move_t> > factorial_move = {};

FIT_TEST_CASE()
{
    const int r = factorial(5);
    FIT_TEST_CHECK(r == 5*4*3*2*1);
}

FIT_TEST_CASE()
{
    const int r = fit::reveal(factorial)(5);
    FIT_TEST_CHECK(r == 5*4*3*2*1);
}

FIT_TEST_CASE()
{
    const int r = fit::fix(fit::result<int>(factorial_constexpr_t()))(5);
    FIT_TEST_CHECK(r == 5*4*3*2*1);
}

FIT_TEST_CASE()
{
    const int r = fit::result<int>(factorial_constexpr)(5);
    FIT_TEST_CHECK(r == 5*4*3*2*1);
}
FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::fix(fit::result<int>(factorial_constexpr_t()))(5) == 5*4*3*2*1);
    FIT_STATIC_TEST_CHECK(fit::result<int>(factorial_constexpr)(5) == 5*4*3*2*1);
}
FIT_TEST_CASE()
{
#if FIT_HAS_GENERIC_LAMBDA
    auto factorial_ = fit::fix([](auto s, auto x) -> decltype(x) { return x == 0 ? 1 : x * s(x-1); });
    int r = fit::result<int>(factorial_)(5);
    FIT_TEST_CHECK(r == 5*4*3*2*1);
#endif
}

FIT_TEST_CASE()
{
    const int r = factorial_move(5);
    FIT_TEST_CHECK(r == 5*4*3*2*1);
    FIT_TEST_CHECK(fit::fix(factorial_move_t())(5) == 5*4*3*2*1);
}
