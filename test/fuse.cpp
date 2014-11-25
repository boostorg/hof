#include <fit/fuse.h>
#include <fit/static.h>
#include <fit/lambda.h>
#include "test.h"

#include <memory>

fit::static_<fit::fuse_adaptor<unary_class> > unary_fuse = {};

FIT_STATIC_AUTO unary_fuse_constexpr = fit::fuse_adaptor<unary_class>();

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::fuse(unary_class())(std::make_tuple(3)));
    FIT_TEST_CHECK(3 == unary_fuse(std::make_tuple(3)));
    int ifu = 3;
    FIT_TEST_CHECK(3 == unary_fuse(std::tuple<int&>(ifu)));

    FIT_STATIC_TEST_CHECK(3 == fit::fuse(unary_class())(std::make_tuple(3)));
    FIT_STATIC_TEST_CHECK(3 == unary_fuse_constexpr(std::make_tuple(3)));
}

FIT_STATIC_AUTO lambda_unary_fuse = fit::fuse(FIT_STATIC_LAMBDA(int x)
{
    return x;
});

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == lambda_unary_fuse(std::make_tuple(3)));
}

struct unary_move
{
    std::unique_ptr<int> i;
    unary_move()
    : i(new int(2))
    {}

    template<class T>
    T operator()(T x) const
    {
        return x + *i;
    }
};

fit::static_<fit::fuse_adaptor<unary_move> > unary_move_fuse = {};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::fuse(unary_move())(std::make_tuple(1)));
    FIT_TEST_CHECK(3 == unary_move_fuse(std::make_tuple(1)));
}
