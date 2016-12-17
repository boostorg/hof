#include <fit/partial.hpp>
#include "test.hpp"

static constexpr fit::static_<fit::partial_adaptor<binary_class> > binary_partial = {};

static constexpr fit::static_<fit::partial_adaptor<unary_class> > unary_partial = {};

static constexpr fit::static_<fit::partial_adaptor<mutable_class> > mutable_partial = {};

static constexpr fit::static_<fit::partial_adaptor<void_class> > void_partial = {};

static constexpr fit::static_<fit::partial_adaptor<mono_class> > mono_partial = {};

static constexpr fit::static_<fit::partial_adaptor<move_class> > move_partial = {};

constexpr fit::partial_adaptor<binary_class> binary_partial_constexpr = {};

constexpr fit::partial_adaptor<unary_class> unary_partial_constexpr = {};

constexpr fit::partial_adaptor<void_class> void_partial_constexpr = {};

constexpr fit::partial_adaptor<mono_class> mono_partial_constexpr = {};

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

FIT_TEST_CASE()
{
    static_assert(noexcept(fit::partial(binary_class{})(1)(2)), "noexcept partial");
}

FIT_TEST_CASE()
{
    auto f = fit::partial(fit::limit_c<2>(binary_class()));
    static_assert(fit::is_callable<decltype(f), int>::value, "Passing the limit is not callable");
    static_assert(fit::is_callable<decltype(f), int, int>::value, "Passing the limit is not callable");
    static_assert(!fit::is_callable<decltype(f), int, int, int>::value, "Passing the limit is not callable");
    static_assert(!fit::is_callable<decltype(f), int, int, int, int>::value, "Passing the limit is not callable");

    auto g = f(0);
    static_assert(fit::is_callable<decltype(g), int>::value, "Passing the limit is not callable");
    static_assert(!fit::is_callable<decltype(g), int, int>::value, "Passing the limit is not callable");
    static_assert(!fit::is_callable<decltype(g), int, int, int>::value, "Passing the limit is not callable");
    static_assert(!fit::is_callable<decltype(g), int, int, int, int>::value, "Passing the limit is not callable");
}
