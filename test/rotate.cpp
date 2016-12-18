#include <fit/rotate.hpp>
#include <fit/placeholders.hpp>
#include <fit/compose.hpp>
#include <fit/repeat.hpp>
#include "test.hpp"

struct head
{
    template<class T, class... Ts>
    constexpr T operator()(T x, Ts&&...) const
    FIT_RETURNS_DEDUCE_NOEXCEPT(x)
    {
        return x;
    }
};

#if FIT_HAS_NOEXCEPT_DEDUCTION
FIT_TEST_CASE()
{
    static_assert(noexcept(fit::rotate(head{})(1, 2, 3, 4)), "noexcept rotate");
    static_assert(noexcept(fit::repeat(std::integral_constant<int, 5>{})(fit::rotate)(head{})(1, 2, 3, 4, 5, 6, 7, 8, 9)), "noexcept rotate");
}
#endif

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(2 == fit::rotate(head{})(1, 2, 3, 4));
    FIT_STATIC_TEST_CHECK(2 == fit::rotate(head{})(1, 2, 3, 4));
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::compose(fit::rotate, fit::rotate)(head{})(1, 2, 3, 4));
    FIT_STATIC_TEST_CHECK(3 == fit::compose(fit::rotate, fit::rotate)(head{})(1, 2, 3, 4));
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(6 == fit::repeat(std::integral_constant<int, 5>{})(fit::rotate)(head{})(1, 2, 3, 4, 5, 6, 7, 8, 9));
    FIT_STATIC_TEST_CHECK(6 == fit::repeat(std::integral_constant<int, 5>{})(fit::rotate)(head{})(1, 2, 3, 4, 5, 6, 7, 8, 9));
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::rotate(fit::_ - fit::_)(2, 5));
    FIT_STATIC_TEST_CHECK(3 == fit::rotate(fit::_ - fit::_)(2, 5));
}

#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define FINAL
#else
#define FINAL final
#endif


struct f FINAL {
    int operator()(int i, void *) const {
        return i;
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::rotate(f())(nullptr, 2) == 2);
}
