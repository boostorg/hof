#include "test.hpp"
#include <fit/capture.hpp>

// TODO: Test empty capture

FIT_TEST_CASE()
{    
    FIT_STATIC_TEST_CHECK(fit::capture(1, 2)(binary_class())() == 3);
    FIT_TEST_CHECK(fit::capture(1, 2)(binary_class())() == 3);

    FIT_STATIC_TEST_CHECK(fit::capture(1)(binary_class())(2) == 3);
    FIT_TEST_CHECK(fit::capture(1)(binary_class())(2) == 3);

    static const int one = 1;
    static const int two = 2;
    FIT_STATIC_TEST_CHECK(fit::capture_forward(one, two)(binary_class())() == 3);
    FIT_TEST_CHECK(fit::capture_forward(one, two)(binary_class())() == 3);
    FIT_TEST_CHECK(fit::capture_forward(1, 2)(binary_class())() == 3);

    FIT_STATIC_TEST_CHECK(fit::capture_forward(one)(binary_class())(two) == 3);
    FIT_TEST_CHECK(fit::capture_forward(1)(binary_class())(2) == 3);

    FIT_STATIC_TEST_CHECK(fit::capture_decay(1, 2)(binary_class())() == 3);
    FIT_TEST_CHECK(fit::capture_decay(1, 2)(binary_class())() == 3);

    FIT_STATIC_TEST_CHECK(fit::capture_decay(1)(binary_class())(2) == 3);
    FIT_TEST_CHECK(fit::capture_decay(1)(binary_class())(2) == 3);
}

struct add_member
{
    int i;

    add_member(int i) : i(i)
    {}

    int add(int j) const
    {
        return i + j;
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::capture(add_member(1), 2)(&add_member::add)() == 3);
    FIT_TEST_CHECK(fit::capture(add_member(1))(&add_member::add)(2) == 3);
}

