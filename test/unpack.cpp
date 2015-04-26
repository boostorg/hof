#include <fit/unpack.h>
#include <fit/static.h>
#include <fit/lambda.h>
#include "test.h"

#include <memory>

fit::static_<fit::unpack_adaptor<unary_class> > unary_unpack = {};
fit::static_<fit::unpack_adaptor<binary_class> > binary_unpack = {};

FIT_STATIC_AUTO unary_unpack_constexpr = fit::unpack_adaptor<unary_class>();
FIT_STATIC_AUTO binary_unpack_constexpr = fit::unpack_adaptor<binary_class>();

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::unpack(unary_class())(std::make_tuple(3)));
    FIT_TEST_CHECK(3 == unary_unpack(std::make_tuple(3)));
    int ifu = 3;
    FIT_TEST_CHECK(3 == unary_unpack(std::tuple<int&>(ifu)));

    FIT_STATIC_TEST_CHECK(3 == fit::unpack(unary_class())(std::make_tuple(3)));
    FIT_STATIC_TEST_CHECK(3 == unary_unpack_constexpr(std::make_tuple(3)));
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::unpack(unary_class())(fit::pack_decay(3)));
    FIT_TEST_CHECK(3 == unary_unpack(fit::pack_decay(3)));
    int ifu = 3;
    FIT_TEST_CHECK(3 == unary_unpack(fit::pack_forward(ifu)));

    FIT_STATIC_TEST_CHECK(3 == fit::unpack(unary_class())(fit::pack_decay(3)));
    FIT_STATIC_TEST_CHECK(3 == unary_unpack_constexpr(fit::pack_decay(3)));
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1, 2)));
    FIT_TEST_CHECK(3 == binary_unpack(std::make_tuple(1, 2)));

    FIT_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1), std::make_tuple(2)));
    FIT_TEST_CHECK(3 == binary_unpack(std::make_tuple(1), std::make_tuple(2)));

    FIT_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));
    FIT_TEST_CHECK(3 == binary_unpack(std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));

    FIT_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(), std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));
    FIT_TEST_CHECK(3 == binary_unpack(std::make_tuple(), std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));

    FIT_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1), std::make_tuple(), std::make_tuple(2), std::make_tuple()));
    FIT_TEST_CHECK(3 == binary_unpack(std::make_tuple(1), std::make_tuple(), std::make_tuple(2), std::make_tuple()));

    FIT_STATIC_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1, 2)));
    FIT_STATIC_TEST_CHECK(3 == binary_unpack_constexpr(std::make_tuple(1, 2)));

    FIT_STATIC_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1), std::make_tuple(2)));
    FIT_STATIC_TEST_CHECK(3 == binary_unpack_constexpr(std::make_tuple(1), std::make_tuple(2)));

    FIT_STATIC_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));
    FIT_STATIC_TEST_CHECK(3 == binary_unpack_constexpr(std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));

    FIT_STATIC_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(), std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));
    FIT_STATIC_TEST_CHECK(3 == binary_unpack_constexpr(std::make_tuple(), std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));

    FIT_STATIC_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1), std::make_tuple(), std::make_tuple(2), std::make_tuple()));
    FIT_STATIC_TEST_CHECK(3 == binary_unpack_constexpr(std::make_tuple(1), std::make_tuple(), std::make_tuple(2), std::make_tuple()));
}

FIT_STATIC_AUTO lambda_unary_unpack = fit::unpack(FIT_STATIC_LAMBDA(int x)
{
    return x;
});

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == lambda_unary_unpack(std::make_tuple(3)));
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == lambda_unary_unpack(fit::pack_decay(3)));
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

fit::static_<fit::unpack_adaptor<unary_move> > unary_move_unpack = {};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::unpack(unary_move())(std::make_tuple(1)));
    FIT_TEST_CHECK(3 == unary_move_unpack(std::make_tuple(1)));
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::unpack(unary_move())(fit::pack_decay(1)));
    FIT_TEST_CHECK(3 == unary_move_unpack(fit::pack_decay(1)));
}
