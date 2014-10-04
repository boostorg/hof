#include <fit/partial.h>
#include "test.h"

fit::static_<fit::partial_adaptor<binary_class> > binary_partial = {};

fit::static_<fit::partial_adaptor<unary_class> > unary_partial = {};

fit::static_<fit::partial_adaptor<mutable_class> > mutable_partial = {};

fit::static_<fit::partial_adaptor<void_class> > void_partial = {};

fit::static_<fit::partial_adaptor<mono_class> > mono_partial = {};

fit::static_<fit::partial_adaptor<move_class> > move_partial = {};

constexpr const fit::partial_adaptor<binary_class> binary_partial_constexpr = {};

constexpr const fit::partial_adaptor<unary_class> unary_partial_constexpr = {};

constexpr const fit::partial_adaptor<void_class> void_partial_constexpr = {};

constexpr const fit::partial_adaptor<mono_class> mono_partial_constexpr = {};

FIT_TEST_CASE()
{
    fit::partial_adaptor<void_class>()(1);

    void_partial(1);
    void_partial()(1);
    FIT_TEST_CHECK(3 == binary_partial(1)(2));
    FIT_TEST_CHECK(3 == binary_partial(1, 2));
    FIT_TEST_CHECK(3 == unary_partial()(3));
    FIT_TEST_CHECK(3 == unary_partial(3));
    FIT_TEST_CHECK(3 == mono_partial(2));
    FIT_TEST_CHECK(3 == mono_partial()(2));

    int i1 = 1;
    FIT_TEST_CHECK(3 == binary_partial(2)(i1));
    FIT_TEST_CHECK(3 == mutable_partial(std::ref(i1))(2));
    FIT_TEST_CHECK(3 == i1);
    int i2 = 1;
    FIT_TEST_CHECK(3 == mutable_partial(i2, 2));
    FIT_TEST_CHECK(3 == i2);
    
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == (move_class()(1, 2)));
    FIT_TEST_CHECK(3 == (move_partial(1, 2)));
    FIT_TEST_CHECK(3 == (move_partial(1)(2)));
    FIT_TEST_CHECK(3 == (fit::partial(move_class())(1)(2)));
    FIT_TEST_CHECK(3 == (fit::partial(move_class())(1, 2)));
}

FIT_TEST_CASE()
{
    void_partial_constexpr(1);
    void_partial_constexpr()(1);
    FIT_STATIC_TEST_CHECK(3 == binary_partial_constexpr(1)(2));
    FIT_STATIC_TEST_CHECK(3 == binary_partial_constexpr(1, 2));
    FIT_STATIC_TEST_CHECK(3 == unary_partial_constexpr()(3));
    FIT_STATIC_TEST_CHECK(3 == unary_partial_constexpr(3));
    FIT_STATIC_TEST_CHECK(3 == mono_partial_constexpr(2));
    FIT_STATIC_TEST_CHECK(3 == mono_partial_constexpr()(2));

}