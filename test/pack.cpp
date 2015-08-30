#include <fit/pack.h>
#include <fit/always.h>
#include <fit/identity.h>
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
    FIT_STATIC_TEST_CHECK(fit::pack(3)(fit::identity) == 3);
    FIT_TEST_CHECK(fit::pack(3)(fit::identity) == 3 );
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

struct empty1
{};

struct empty2
{};

FIT_TEST_CASE()
{
    static_assert(fit::detail::is_default_constructible<empty1, empty2>::value, "Not default constructible");
}

FIT_TEST_CASE()
{
    static constexpr auto p = fit::pack(empty1());
    FIT_TEST_CHECK(p(fit::always(0)) == 0);
    FIT_STATIC_TEST_CHECK(p(fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");

}

FIT_TEST_CASE()
{
    static constexpr auto p = fit::pack(empty1(), empty2());
    FIT_TEST_CHECK(p(fit::always(0)) == 0);
    FIT_STATIC_TEST_CHECK(p(fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");
}

FIT_TEST_CASE()
{
    static constexpr auto p = fit::pack(fit::pack(), fit::pack());
    FIT_TEST_CHECK(p(fit::always(0)) == 0);
    FIT_STATIC_TEST_CHECK(p(fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");
}

FIT_TEST_CASE()
{
    static constexpr auto p = fit::pack(empty1(), empty2(), empty1());
    FIT_TEST_CHECK(p(fit::always(0)) == 0);
    FIT_STATIC_TEST_CHECK(p(fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");
}

FIT_TEST_CASE()
{
    static constexpr auto p = fit::pack(empty1(), fit::pack(empty1(), empty2()));
    FIT_TEST_CHECK(p(fit::always(0)) == 0);
    FIT_STATIC_TEST_CHECK(p(fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");
}

FIT_TEST_CASE()
{
    static constexpr auto p = fit::pack(fit::pack(), fit::pack(fit::pack()), empty1(), fit::pack(empty1(), empty2()));
    FIT_TEST_CHECK(p(fit::always(0)) == 0);
    FIT_STATIC_TEST_CHECK(p(fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");
}
