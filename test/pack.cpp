#include <fit/pack.h>
#include <fit/always.h>
#include "test.h"

FIT_TEST_CASE()
{
    static_assert(fit::pack(1, 2)(binary_class()) == 3, "static pack test failed");
    FIT_TEST_CHECK(fit::pack(1, 2)(binary_class()) == 3 );

    static_assert(fit::pack_decay(1, 2)(binary_class()) == 3, "static pack_decay test failed");
    FIT_TEST_CHECK(fit::pack_decay(1, 2)(binary_class()) == 3 );

    static_assert(fit::pack_forward(1, 2)(binary_class()) == 3, "static pack_forward test failed");
    FIT_TEST_CHECK(fit::pack_forward(1, 2)(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    static_assert(fit::pack()(fit::always(3)) == 3,  "static unary pack test failed");
    FIT_TEST_CHECK(fit::pack()(fit::always(3)) == 3 );
}

FIT_TEST_CASE()
{
    static_assert(fit::pack_join(fit::pack(1), fit::pack(2))(binary_class()) == 3, "static pack test failed");
    FIT_TEST_CHECK(fit::pack_join(fit::pack(1), fit::pack(2))(binary_class()) == 3 );

    static_assert(fit::pack_join(fit::pack_decay(1), fit::pack_decay(2))(binary_class()) == 3, "static pack_decay test failed");
    FIT_TEST_CHECK(fit::pack_join(fit::pack_decay(1), fit::pack_decay(2))(binary_class()) == 3 );

    static_assert(fit::pack_join(fit::pack_forward(1), fit::pack_forward(2))(binary_class()) == 3, "static pack_forward test failed");
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(1), fit::pack_forward(2))(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    static_assert(fit::pack_join(fit::pack(), fit::pack(1, 2))(binary_class()) == 3, "static pack test failed");
    FIT_TEST_CHECK(fit::pack_join(fit::pack(), fit::pack(1, 2))(binary_class()) == 3 );

    static_assert(fit::pack_join(fit::pack_decay(), fit::pack_decay(1, 2))(binary_class()) == 3, "static pack_decay test failed");
    FIT_TEST_CHECK(fit::pack_join(fit::pack_decay(), fit::pack_decay(1, 2))(binary_class()) == 3 );

    static_assert(fit::pack_join(fit::pack_forward(), fit::pack_forward(1, 2))(binary_class()) == 3, "static pack_forward test failed");
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(), fit::pack_forward(1, 2))(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    static_assert(fit::pack_join(fit::pack(1, 2), fit::pack())(binary_class()) == 3, "static pack test failed");
    FIT_TEST_CHECK(fit::pack_join(fit::pack(1, 2), fit::pack())(binary_class()) == 3 );

    static_assert(fit::pack_join(fit::pack_decay(1, 2), fit::pack_decay())(binary_class()) == 3, "static pack_decay test failed");
    FIT_TEST_CHECK(fit::pack_join(fit::pack_decay(1, 2), fit::pack_decay())(binary_class()) == 3 );

    static_assert(fit::pack_join(fit::pack_forward(1, 2), fit::pack_forward())(binary_class()) == 3, "static pack_forward test failed");
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(1, 2), fit::pack_forward())(binary_class()) == 3 );
}
