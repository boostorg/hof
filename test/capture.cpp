#include <fit/capture.hpp>
#include <fit/identity.hpp>
#include "test.hpp"

FIT_TEST_CASE()
{    
    FIT_STATIC_TEST_CHECK(fit::capture_basic(1, 2)(binary_class())() == 3);
    FIT_TEST_CHECK(fit::capture_basic(1, 2)(binary_class())() == 3);

    FIT_STATIC_TEST_CHECK(fit::capture_basic(1)(binary_class())(2) == 3);
    FIT_TEST_CHECK(fit::capture_basic(1)(binary_class())(2) == 3);

    FIT_STATIC_TEST_CHECK(fit::capture_basic()(binary_class())(1, 2) == 3);
    FIT_TEST_CHECK(fit::capture_basic()(binary_class())(1, 2) == 3);

    static const int one = 1;
    static const int two = 2;
    FIT_STATIC_TEST_CHECK(fit::capture_forward(one, two)(binary_class())() == 3);
    FIT_TEST_CHECK(fit::capture_forward(one, two)(binary_class())() == 3);
    FIT_TEST_CHECK(fit::capture_forward(1, 2)(binary_class())() == 3);

    FIT_STATIC_TEST_CHECK(fit::capture_forward(one)(binary_class())(two) == 3);
    FIT_TEST_CHECK(fit::capture_forward(1)(binary_class())(2) == 3);

    FIT_STATIC_TEST_CHECK(fit::capture_forward()(binary_class())(one, two) == 3);
    FIT_TEST_CHECK(fit::capture_forward()(binary_class())(one, two) == 3);

    FIT_STATIC_TEST_CHECK(fit::capture(1, 2)(binary_class())() == 3);
    FIT_TEST_CHECK(fit::capture(1, 2)(binary_class())() == 3);

    FIT_STATIC_TEST_CHECK(fit::capture(1)(binary_class())(2) == 3);
    FIT_TEST_CHECK(fit::capture(1)(binary_class())(2) == 3);

    FIT_STATIC_TEST_CHECK(fit::capture()(binary_class())(1, 2) == 3);
    FIT_TEST_CHECK(fit::capture()(binary_class())(1, 2) == 3);
}

struct add_member
{
    int i;

    add_member(int ip) : i(ip)
    {}

    int add(int j) const
    {
        return i + j;
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::capture_basic(add_member(1), 2)(&add_member::add)() == 3);
    FIT_TEST_CHECK(fit::capture_basic(add_member(1))(&add_member::add)(2) == 3);
}

FIT_TEST_CASE()
{
    auto id = fit::identity;
    auto f = fit::capture(fit::identity)(fit::identity);
    static_assert(FIT_IS_DEFAULT_CONSTRUCTIBLE(decltype(id)), "Id not default constructible");
    static_assert(FIT_IS_DEFAULT_CONSTRUCTIBLE(decltype(f)), "Not default constructible");
    f();
}
#if FIT_HAS_NOEXCEPT_DEDUCTION
FIT_TEST_CASE()
{
    static_assert(noexcept(fit::capture(fit::identity)(fit::identity)()), "noexcept capture");
    static_assert(noexcept(fit::capture_basic(fit::identity)(fit::identity)()), "noexcept capture");
    static_assert(noexcept(fit::capture_forward(fit::identity)(fit::identity)()), "noexcept capture");
}
#endif
FIT_TEST_CASE()
{
    auto f = fit::capture_basic(fit::identity)(fit::identity);
    f();
}

FIT_TEST_CASE()
{
    auto f = fit::capture_forward(fit::identity)(fit::identity);
    f();
}

FIT_TEST_CASE()
{
    auto f = fit::capture(fit::identity)(add_member{1});
    static_assert(!fit::is_callable<decltype(f), int>::value, "Not sfinae friendly");
}

