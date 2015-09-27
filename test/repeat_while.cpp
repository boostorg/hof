#include <fit/repeat_while.h>
#include "test.h"

// TODO: Test default construction, and static initialization

struct increment
{
    template<class T>
    constexpr std::integral_constant<int, T::value + 1> operator()(T) const
    {
        return std::integral_constant<int, T::value + 1>();
    }
};

struct not_6
{
    template<class T>
    constexpr std::integral_constant<bool, (T::value != 6)> 
    operator()(T) const
    {
        return std::integral_constant<bool, (T::value != 6)>();
    }
};

FIT_TEST_CASE()
{
    static_assert
    (
        std::is_same<
            std::integral_constant<int, 6>, 
            decltype(fit::repeat_while(not_6())(increment())(std::integral_constant<int, 1>()))
        >::value,
        "Error"
    );

    std::integral_constant<int, 6> x = fit::repeat_while(not_6())(increment())(std::integral_constant<int, 1>());
    fit::test::unused(x);
}
