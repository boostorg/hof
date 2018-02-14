#include <fit/decay.hpp>
#include "test.hpp"

#define CHECK_DECAY(T) \
    STATIC_ASSERT_SAME(decltype(fit::decay(std::declval<T>())), std::decay<T>::type)

FIT_TEST_CASE()
{
    CHECK_DECAY(int);
    CHECK_DECAY(int*);
    CHECK_DECAY(int&);
    CHECK_DECAY(int&&);
    CHECK_DECAY(const int&);
    CHECK_DECAY(int[2]);
    CHECK_DECAY(int(int));
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::decay(3) == 3);
    FIT_STATIC_TEST_CHECK(fit::decay(3) == 3);
}
