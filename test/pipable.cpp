#include <fit/pipable.h>
#include <fit/static.h>
#include "test.h"

fit::static_<fit::pipable_adaptor<binary_class> > binary_pipable = {};

fit::static_<fit::pipable_adaptor<unary_class> > unary_pipable = {};

fit::static_<fit::pipable_adaptor<mutable_class> > mutable_pipable = {};

fit::static_<fit::pipable_adaptor<void_class> > void_pipable = {};

fit::static_<fit::pipable_adaptor<mono_class> > mono_pipable = {};

fit::static_<fit::pipable_adaptor<move_class> > move_pipable = {};

constexpr fit::pipable_adaptor<void_class> void_pipable_constexpr = {};

constexpr fit::pipable_adaptor<binary_class> binary_pipable_constexpr = {};

constexpr fit::pipable_adaptor<unary_class> unary_pipable_constexpr = {};


FIT_TEST_CASE()
{
    void_pipable(1);
    1 | void_pipable;
    FIT_TEST_CHECK(3 == (1 | binary_pipable(2)));
    FIT_TEST_CHECK(3 == (binary_pipable(1, 2)));
    FIT_TEST_CHECK(3 == (3 | unary_pipable));
    FIT_TEST_CHECK(3 == (3 | unary_pipable()));
    FIT_TEST_CHECK(3 == (unary_pipable(3)));
    int i1 = 1;
    FIT_TEST_CHECK(3 == (2 | binary_pipable(i1)));
    FIT_TEST_CHECK(3 == (i1 | mutable_pipable(2)));
    FIT_TEST_CHECK(3 == (i1));
    int i2 = 1;
    FIT_TEST_CHECK(3 == (mutable_pipable(i2, 2)));
    FIT_TEST_CHECK(3 == (i2));
    FIT_TEST_CHECK(3 == (mono_pipable(2)));
    FIT_TEST_CHECK(3 == (2| mono_pipable));
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == (move_class()(1, 2)));
    FIT_TEST_CHECK(3 == (move_pipable(1, 2)));
    FIT_TEST_CHECK(3 == (1 | move_pipable(2)));
    FIT_TEST_CHECK(3 == (1 | fit::pipable(move_class())(2)));
    FIT_TEST_CHECK(3 == (fit::pipable(move_class())(1, 2)));
}

FIT_TEST_CASE()
{
    void_pipable_constexpr(1);
    1 | void_pipable_constexpr;
    FIT_STATIC_TEST_CHECK(3 == (1 | binary_pipable_constexpr(2)));
    FIT_STATIC_TEST_CHECK(3 == (binary_pipable_constexpr(1, 2)));
    FIT_STATIC_TEST_CHECK(3 == (3 | unary_pipable_constexpr));
    FIT_STATIC_TEST_CHECK(3 == (3 | unary_pipable_constexpr()));
    FIT_STATIC_TEST_CHECK(3 == (unary_pipable_constexpr(3)));
}

