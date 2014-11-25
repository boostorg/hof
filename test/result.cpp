#include <fit/result.h>
#include <fit/static.h>
#include "test.h"


static constexpr fit::result_adaptor<int, unary_class> unary_int = {};

FIT_TEST_CASE()
{
    STATIC_ASSERT_SAME(decltype(unary_int(false)), int);
    FIT_TEST_CHECK(unary_int(false) == 0);
    FIT_STATIC_TEST_CHECK(unary_int(false) == 0);
}

static constexpr fit::result_adaptor<void, unary_class> unary_void = {};

FIT_TEST_CASE()
{
    STATIC_ASSERT_SAME(decltype(unary_void(false)), void);
}
