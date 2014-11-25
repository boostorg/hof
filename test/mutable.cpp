#include <fit/mutable.h>
#include <fit/lazy.h>
#include <fit/detail/move.h>
#include <memory>
#include "test.h"

struct mutable_fun
{
    int x;
    mutable_fun() : x(1)
    {}

    int operator()(int i)
    {
        x+=i;
        return x;
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::mutable_(mutable_fun())(3) == 4);
}

FIT_TEST_CASE()
{
    auto mut_fun = mutable_fun();
    auto by_5 = fit::lazy(fit::mutable_(std::ref(mut_fun)))(5);
    FIT_TEST_CHECK(by_5() == 6);
    FIT_TEST_CHECK(by_5() == 11);
}

struct mutable_move_fun
{
    std::unique_ptr<int> x;
    mutable_move_fun() : x(new int(1))
    {}

    int operator()(int i)
    {
        *x+=i;
        return *x;
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::mutable_(mutable_move_fun())(3) == 4);
}

FIT_TEST_CASE()
{
    auto mut_fun = mutable_move_fun();
    auto by_5 = fit::lazy(fit::mutable_(fit::move(mut_fun)))(5);
    FIT_TEST_CHECK(by_5() == 6);
    FIT_TEST_CHECK(by_5() == 11);
}

