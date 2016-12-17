#include <fit/if.hpp>
#include "test.hpp"

#include <fit/conditional.hpp>
#include <fit/placeholders.hpp>


struct is_5
{
    template<class T>
    constexpr bool operator()(T i) const
    {
        return i == 5;
    }
};

struct is_not_5
{
    template<class T>
    constexpr bool operator()(T i) const
    {
        return i != 5;
    }
};

template<class F>
struct test_int
{
    template<class T>
    constexpr bool operator()(T x) const
    {
        return fit::conditional(
            fit::if_(std::is_integral<T>())(F()),
            fit::always(true)
        )(x);
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(test_int<is_5>()(5));
    FIT_TEST_CHECK(test_int<is_5>()(5L));
    FIT_TEST_CHECK(test_int<is_5>()(5.0));
    FIT_TEST_CHECK(test_int<is_5>()(6.0));

    FIT_TEST_CHECK(test_int<is_not_5>()(6));
    FIT_TEST_CHECK(test_int<is_not_5>()(6L));
    FIT_TEST_CHECK(test_int<is_not_5>()(5.0));
    FIT_TEST_CHECK(test_int<is_not_5>()(6.0));

    FIT_STATIC_TEST_CHECK(test_int<is_5>()(5));
    FIT_STATIC_TEST_CHECK(test_int<is_5>()(5L));
    FIT_STATIC_TEST_CHECK(test_int<is_5>()(5.0));
    FIT_STATIC_TEST_CHECK(test_int<is_5>()(6.0));

    FIT_STATIC_TEST_CHECK(test_int<is_not_5>()(6));
    FIT_STATIC_TEST_CHECK(test_int<is_not_5>()(6L));
    FIT_STATIC_TEST_CHECK(test_int<is_not_5>()(5.0));
    FIT_STATIC_TEST_CHECK(test_int<is_not_5>()(6.0));
}

template<class F>
struct test_int_c
{
    template<class T>
    constexpr bool operator()(T x) const
    {
        return fit::conditional(
            fit::if_c<std::is_integral<T>::value>(F()),
            fit::always(true)
        )(x);
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(test_int_c<is_5>()(5));
    FIT_TEST_CHECK(test_int_c<is_5>()(5L));
    FIT_TEST_CHECK(test_int_c<is_5>()(5.0));
    FIT_TEST_CHECK(test_int_c<is_5>()(6.0));

    FIT_TEST_CHECK(test_int_c<is_not_5>()(6));
    FIT_TEST_CHECK(test_int_c<is_not_5>()(6L));
    FIT_TEST_CHECK(test_int_c<is_not_5>()(5.0));
    FIT_TEST_CHECK(test_int_c<is_not_5>()(6.0));

    FIT_STATIC_TEST_CHECK(test_int_c<is_5>()(5));
    FIT_STATIC_TEST_CHECK(test_int_c<is_5>()(5L));
    FIT_STATIC_TEST_CHECK(test_int_c<is_5>()(5.0));
    FIT_STATIC_TEST_CHECK(test_int_c<is_5>()(6.0));

    FIT_STATIC_TEST_CHECK(test_int_c<is_not_5>()(6));
    FIT_STATIC_TEST_CHECK(test_int_c<is_not_5>()(6L));
    FIT_STATIC_TEST_CHECK(test_int_c<is_not_5>()(5.0));
    FIT_STATIC_TEST_CHECK(test_int_c<is_not_5>()(6.0));
}

struct sum_f
{
    template<class T>
    constexpr int operator()(T x, T y) const
    {
        return fit::conditional(
            fit::if_(std::is_integral<T>())(fit::_ + fit::_),
            fit::always(0)
        )(x, y);
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(sum_f()(1, 2) == 3);
    FIT_TEST_CHECK(sum_f()(1.0, 2.0) == 0);
    FIT_TEST_CHECK(sum_f()("", "") == 0);

    FIT_STATIC_TEST_CHECK(sum_f()(1, 2) == 3);
    FIT_STATIC_TEST_CHECK(sum_f()("", "") == 0);
}


struct sum_f_c
{
    template<class T>
    constexpr int operator()(T x, T y) const
    {
        return fit::conditional(
            fit::if_c<std::is_integral<T>::value>(fit::_ + fit::_),
            fit::always(0)
        )(x, y);
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(sum_f_c()(1, 2) == 3);
    FIT_TEST_CHECK(sum_f_c()(1.0, 2.0) == 0);
    FIT_TEST_CHECK(sum_f_c()("", "") == 0);

    FIT_STATIC_TEST_CHECK(sum_f_c()(1, 2) == 3);
    FIT_STATIC_TEST_CHECK(sum_f_c()("", "") == 0);
}

FIT_TEST_CASE()
{
    static_assert(noexcept(fit::if_(std::is_integral<int>())(fit::identity)(1)), "noexcept if");
}
