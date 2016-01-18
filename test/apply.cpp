#include <fit/apply.hpp>
#include "test.hpp"

FIT_TEST_CASE()
{    
    FIT_STATIC_TEST_CHECK(fit::apply(binary_class(), 1, 2) == 3);
    FIT_TEST_CHECK(fit::apply(binary_class(), 1, 2) == 3);
}

struct member_sum_f
{
    int i;
    constexpr member_sum_f(int x) : i(x)
    {}

    constexpr int add(int x) const
    {
        return i+x;
    }
};

struct member_sum_f_derived
: member_sum_f
{
    constexpr member_sum_f_derived(int x) : member_sum_f(x)
    {}
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::apply(&member_sum_f::add, member_sum_f(1), 2) == 3);
    FIT_TEST_CHECK(fit::apply(&member_sum_f::add, member_sum_f_derived(1), 2) == 3);

#ifdef __clang__
    FIT_STATIC_TEST_CHECK(fit::apply(&member_sum_f::add, member_sum_f(1), 2) == 3);
    FIT_STATIC_TEST_CHECK(fit::apply(&member_sum_f::add, member_sum_f_derived(1), 2) == 3);
#endif

    static_assert(std::is_base_of<member_sum_f, member_sum_f>::value, "Base of failed");
    std::unique_ptr<member_sum_f> msp(new member_sum_f(1));
    FIT_TEST_CHECK(fit::apply(&member_sum_f::add, msp, 2) == 3);

    std::unique_ptr<member_sum_f_derived> mspd(new member_sum_f_derived(1));
    FIT_TEST_CHECK(fit::apply(&member_sum_f::add, mspd, 2) == 3);
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::apply(&member_sum_f::i, member_sum_f(3)) == 3);
    FIT_TEST_CHECK(fit::apply(&member_sum_f::i, member_sum_f_derived(3)) == 3);

#ifdef __clang__
    FIT_STATIC_TEST_CHECK(fit::apply(&member_sum_f::i, member_sum_f(3)) == 3);
    FIT_STATIC_TEST_CHECK(fit::apply(&member_sum_f::i, member_sum_f_derived(3)) == 3);
#endif

    std::unique_ptr<member_sum_f> msp(new member_sum_f(3));
    FIT_TEST_CHECK(fit::apply(&member_sum_f::i, msp) == 3);

    std::unique_ptr<member_sum_f_derived> mspd(new member_sum_f_derived(3));
    FIT_TEST_CHECK(fit::apply(&member_sum_f::i, mspd) == 3);
}
