#include <fit/pack.h>
#include <fit/always.h>
#include <memory>
#include "test.h"

FIT_TEST_CASE()
{
    auto p1 = fit::pack(1, 2);
    auto p2 = p1;
    FIT_TEST_CHECK(p2(binary_class()) == p1(binary_class()));
    
    FIT_STATIC_TEST_CHECK(fit::pack(1, 2)(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack(1, 2)(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_decay(1, 2)(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_decay(1, 2)(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_forward(1, 2)(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_forward(1, 2)(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::pack()(fit::always(3)) == 3);
    FIT_TEST_CHECK(fit::pack()(fit::always(3)) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack(1), fit::pack(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack(1), fit::pack(2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_decay(1), fit::pack_decay(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_decay(1), fit::pack_decay(2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_forward(1), fit::pack_forward(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(1), fit::pack_forward(2))(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack(), fit::pack(1, 2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack(), fit::pack(1, 2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_decay(), fit::pack_decay(1, 2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_decay(), fit::pack_decay(1, 2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_forward(), fit::pack_forward(1, 2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(), fit::pack_forward(1, 2))(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack(1, 2), fit::pack())(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack(1, 2), fit::pack())(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_decay(1, 2), fit::pack_decay())(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_decay(1, 2), fit::pack_decay())(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_forward(1, 2), fit::pack_forward())(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(1, 2), fit::pack_forward())(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack(1), fit::pack(), fit::pack(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack(1), fit::pack(), fit::pack(2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_decay(1), fit::pack_decay(), fit::pack_decay(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_decay(1), fit::pack_decay(), fit::pack_decay(2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_forward(1), fit::pack_forward(), fit::pack_forward(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(1), fit::pack_forward(), fit::pack_forward(2))(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack(), fit::pack(1), fit::pack(), fit::pack(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack(), fit::pack(1), fit::pack(), fit::pack(2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_decay(), fit::pack_decay(1), fit::pack_decay(), fit::pack_decay(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_decay(), fit::pack_decay(1), fit::pack_decay(), fit::pack_decay(2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_forward(), fit::pack_forward(1), fit::pack_forward(), fit::pack_forward(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(), fit::pack_forward(1), fit::pack_forward(), fit::pack_forward(2))(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack(1), fit::pack(), fit::pack(2), fit::pack())(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack(1), fit::pack(), fit::pack(2), fit::pack())(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_decay(1), fit::pack_decay(), fit::pack_decay(2), fit::pack_decay())(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_decay(1), fit::pack_decay(), fit::pack_decay(2), fit::pack_decay())(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_forward(1), fit::pack_forward(), fit::pack_forward(2), fit::pack_forward())(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(1), fit::pack_forward(), fit::pack_forward(2), fit::pack_forward())(binary_class()) == 3 );
}

struct deref
{
    int operator()(const std::unique_ptr<int>& i) const
    {
        return *i;
    }
};

FIT_TEST_CASE()
{
    std::unique_ptr<int> i(new int(3));
    FIT_TEST_CHECK(fit::pack(i)(deref()) == 3);
    FIT_TEST_CHECK(fit::pack(std::unique_ptr<int>(new int(3)))(deref()) == 3);
    FIT_TEST_CHECK(fit::pack_forward(std::unique_ptr<int>(new int(3)))(deref()) == 3);
    FIT_TEST_CHECK(fit::pack_decay(std::unique_ptr<int>(new int(3)))(deref()) == 3);
    auto p = fit::pack(std::unique_ptr<int>(new int(3)));
    FIT_TEST_CHECK(p(deref()) == 3);

    FIT_TEST_CHECK(fit::pack_join(fit::pack(), fit::pack(std::unique_ptr<int>(new int(3))))(deref()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(), fit::pack_forward(std::unique_ptr<int>(new int(3))))(deref()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_decay(), fit::pack_decay(std::unique_ptr<int>(new int(3))))(deref()) == 3);
    // FIT_TEST_CHECK(p(deref()) == 3);
}

class empty1
{};

class empty2
{};
#if FIT_PACK_HAS_EBO
FIT_TEST_CASE()
{
    auto p1 = fit::pack(empty1());
    p1(fit::always(0));
    static_assert(std::is_empty<decltype(p1)>::value, "Pack not empty");

    auto p2 = fit::pack(empty1(), empty2());
    p2(fit::always(0));
    static_assert(std::is_empty<decltype(p2)>::value, "Pack not empty");

    auto p3 = fit::pack(empty1(), empty2(), empty1());
    p2(fit::always(0));
    static_assert(std::is_empty<decltype(p3)>::value, "Pack not empty");
}
#endif
