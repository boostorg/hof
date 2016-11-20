#include <fit/repeat_while.hpp>
#include <fit/reveal.hpp>
#include "test.hpp"

// TODO: Test default construction, and static initialization

struct increment_constant
{
    template<class T>
    constexpr std::integral_constant<int, T::value + 1> operator()(T) const
    {
        return std::integral_constant<int, T::value + 1>();
    }
};

struct increment
{
    template<class T>
    constexpr T operator()(T x) const
    {
        return x + 1;
    }
};

struct not_6_constant
{
    template<class T>
    constexpr std::integral_constant<bool, (T::value != 6)> 
    operator()(T) const
    {
        return std::integral_constant<bool, (T::value != 6)>();
    }
};

struct not_6
{
    template<class T>
    constexpr bool operator()(T x) const
    {
        return x != 6;
    }
};

struct not_limit
{
    template<class T>
    constexpr bool operator()(T x) const
    {
        return x != (FIT_RECURSIVE_CONSTEXPR_DEPTH+4);
    }
};

FIT_TEST_CASE()
{
    static_assert
    (
        std::is_same<
            std::integral_constant<int, 6>, 
            decltype(fit::repeat_while(not_6_constant())(increment_constant())(std::integral_constant<int, 1>()))
        >::value,
        "Error"
    );

    std::integral_constant<int, 6> x = fit::repeat_while(not_6_constant())(increment_constant())(std::integral_constant<int, 1>());
    fit::test::unused(x);
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::repeat_while(not_6())(increment())(1) == 6);
    FIT_TEST_CHECK(fit::repeat_while(not_6())(increment())(1) == 6);
    FIT_TEST_CHECK(fit::reveal(fit::repeat_while(not_6())(increment()))(1) == 6);
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::repeat_while(not_limit())(increment())(1) == FIT_RECURSIVE_CONSTEXPR_DEPTH+4);
#if FIT_HAS_RELAXED_CONSTEXPR
    FIT_STATIC_TEST_CHECK(fit::repeat_while(not_limit())(increment())(1) == FIT_RECURSIVE_CONSTEXPR_DEPTH+4);
#endif
}
