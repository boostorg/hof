#include <fit/placeholders.h>
#include "test.h"

// TODO: Test assign operators

#if FIT_HAS_STATIC_TEST_CHECK
#define FIT_PLACEHOLDER_TEST_CONSTEXPR constexpr
#else
#define FIT_PLACEHOLDER_TEST_CONSTEXPR
#endif

struct square
{
    template<class T>
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto operator()(T x) const FIT_RETURNS(x*x);
};

FIT_TEST_CASE()
{
    const auto x_square_add = 2 + (4*4);
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_square_add = fit::_1 + fit::lazy(square())(fit::_2);
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_square_add)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_square_add(2, 4) == x_square_add);
    FIT_TEST_CHECK(f_square_add(2, 4) == x_square_add);
}

FIT_TEST_CASE()
{
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_invoke_2 = fit::_1(3);
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_invoke_2)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_invoke_2(square()) == 9);
    FIT_TEST_CHECK(f_invoke_2(square()) == 9);
}

FIT_TEST_CASE()
{
    const auto x_add = 2 + 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_add = fit::_1 + fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_add)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_add(2, 1) == x_add);
    FIT_TEST_CHECK(f_add(2, 1) == x_add);

    const auto x_subtract = 2 - 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_subtract = fit::_1 - fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_subtract)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_subtract(2, 1) == x_subtract);
    FIT_TEST_CHECK(f_subtract(2, 1) == x_subtract);

    const auto x_multiply = 2 * 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_multiply = fit::_1 * fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_multiply)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_multiply(2, 1) == x_multiply);
    FIT_TEST_CHECK(f_multiply(2, 1) == x_multiply);

    const auto x_divide = 2 / 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_divide = fit::_1 / fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_divide)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_divide(2, 1) == x_divide);
    FIT_TEST_CHECK(f_divide(2, 1) == x_divide);

    const auto x_remainder = 2 % 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_remainder = fit::_1 % fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_remainder)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_remainder(2, 1) == x_remainder);
    FIT_TEST_CHECK(f_remainder(2, 1) == x_remainder);

    const auto x_shift_right = 2 >> 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_shift_right = fit::_1 >> fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_shift_right)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_shift_right(2, 1) == x_shift_right);
    FIT_TEST_CHECK(f_shift_right(2, 1) == x_shift_right);

    const auto x_shift_left = 2 << 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_shift_left = fit::_1 << fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_shift_left)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_shift_left(2, 1) == x_shift_left);
    FIT_TEST_CHECK(f_shift_left(2, 1) == x_shift_left);

    const auto x_greater_than = 2 > 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_greater_than = fit::_1 > fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_greater_than)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_greater_than(2, 1) == x_greater_than);
    FIT_TEST_CHECK(f_greater_than(2, 1) == x_greater_than);

    const auto x_less_than = 2 < 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_less_than = fit::_1 < fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_less_than)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_less_than(2, 1) == x_less_than);
    FIT_TEST_CHECK(f_less_than(2, 1) == x_less_than);

    const auto x_less_than_equal = 2 <= 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_less_than_equal = fit::_1 <= fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_less_than_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_less_than_equal(2, 1) == x_less_than_equal);
    FIT_TEST_CHECK(f_less_than_equal(2, 1) == x_less_than_equal);

    const auto x_greater_than_equal = 2 >= 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_greater_than_equal = fit::_1 >= fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_greater_than_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_greater_than_equal(2, 1) == x_greater_than_equal);
    FIT_TEST_CHECK(f_greater_than_equal(2, 1) == x_greater_than_equal);

    const auto x_equal = 2 == 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_equal = fit::_1 == fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_equal(2, 1) == x_equal);
    FIT_TEST_CHECK(f_equal(2, 1) == x_equal);

    const auto x_not_equal = 2 != 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_not_equal = fit::_1 != fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_not_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_not_equal(2, 1) == x_not_equal);
    FIT_TEST_CHECK(f_not_equal(2, 1) == x_not_equal);

    const auto x_bit_and = 2 & 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_bit_and = fit::_1 & fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_bit_and)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_bit_and(2, 1) == x_bit_and);
    FIT_TEST_CHECK(f_bit_and(2, 1) == x_bit_and);

    const auto x_xor_ = 2 ^ 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_xor_ = fit::_1 ^ fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_xor_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_xor_(2, 1) == x_xor_);
    FIT_TEST_CHECK(f_xor_(2, 1) == x_xor_);

    const auto x_bit_or = 2 | 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_bit_or = fit::_1 | fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_bit_or)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_bit_or(2, 1) == x_bit_or);
    FIT_TEST_CHECK(f_bit_or(2, 1) == x_bit_or);

    const auto x_and_ = true && false;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_and_ = fit::_1 && fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_and_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_and_(true, false) == x_and_);
    FIT_TEST_CHECK(f_and_(true, false) == x_and_);

    const auto x_or_ = true || false;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_or_ = fit::_1 || fit::_2;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_or_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_or_(true, false) == x_or_);
    FIT_TEST_CHECK(f_or_(true, false) == x_or_);
}

FIT_TEST_CASE()
{

    const auto x_not_ = !false;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_not_ = !fit::_1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_not_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_not_(false) == x_not_);
    FIT_TEST_CHECK(f_not_(false) == x_not_);

    const auto x_compl_ = ~2;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_compl_ = ~fit::_1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_compl_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_compl_(2) == x_compl_);
    FIT_TEST_CHECK(f_compl_(2) == x_compl_);

    const auto x_unary_plus = +2;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_unary_plus = +fit::_1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_unary_plus)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_unary_plus(2) == x_unary_plus);
    FIT_TEST_CHECK(f_unary_plus(2) == x_unary_plus);

    const auto x_unary_subtract = -2;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_unary_subtract = -fit::_1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_unary_subtract)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_unary_subtract(2) == x_unary_subtract);
    FIT_TEST_CHECK(f_unary_subtract(2) == x_unary_subtract);

    const auto x_dereference = 2;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_dereference = *fit::_1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_dereference)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_dereference(&x_dereference) == x_dereference);
    FIT_TEST_CHECK(f_dereference(&x_dereference) == x_dereference);

    // TODO: Test FIT_PLACEHOLDER_TEST_CONSTEXPR increment and decrement
#ifndef _MSC_VER
    auto x_increment = 2;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_increment = ++fit::_1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_increment)>::value, "Not copyable");
#endif
    f_increment(x_increment);
    FIT_TEST_CHECK(x_increment == 3);

    auto x_decrement = 2;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_decrement = --fit::_1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_decrement)>::value, "Not copyable");
#endif
    f_decrement(x_decrement);
    FIT_TEST_CHECK(x_decrement == 1);
#endif

    // TODO: Test post increment and decrement
}


FIT_TEST_CASE()
{
    const auto x_add = 2 + 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_add = fit::_ + fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_add)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_add(2, 1) == x_add);
    FIT_TEST_CHECK(f_add(2, 1) == x_add);

    const auto x_subtract = 2 - 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_subtract = fit::_ - fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_subtract)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_subtract(2, 1) == x_subtract);
    FIT_TEST_CHECK(f_subtract(2, 1) == x_subtract);

    const auto x_multiply = 2 * 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_multiply = fit::_ * fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_multiply)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_multiply(2, 1) == x_multiply);
    FIT_TEST_CHECK(f_multiply(2, 1) == x_multiply);

    const auto x_divide = 2 / 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_divide = fit::_ / fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_divide)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_divide(2, 1) == x_divide);
    FIT_TEST_CHECK(f_divide(2, 1) == x_divide);

    const auto x_remainder = 2 % 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_remainder = fit::_ % fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_remainder)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_remainder(2, 1) == x_remainder);
    FIT_TEST_CHECK(f_remainder(2, 1) == x_remainder);

    const auto x_shift_right = 2 >> 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_shift_right = fit::_ >> fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_shift_right)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_shift_right(2, 1) == x_shift_right);
    FIT_TEST_CHECK(f_shift_right(2, 1) == x_shift_right);

    const auto x_shift_left = 2 << 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_shift_left = fit::_ << fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_shift_left)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_shift_left(2, 1) == x_shift_left);
    FIT_TEST_CHECK(f_shift_left(2, 1) == x_shift_left);

    const auto x_greater_than = 2 > 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_greater_than = fit::_ > fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_greater_than)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_greater_than(2, 1) == x_greater_than);
    FIT_TEST_CHECK(f_greater_than(2, 1) == x_greater_than);

    const auto x_less_than = 2 < 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_less_than = fit::_ < fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_less_than)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_less_than(2, 1) == x_less_than);
    FIT_TEST_CHECK(f_less_than(2, 1) == x_less_than);

    const auto x_less_than_equal = 2 <= 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_less_than_equal = fit::_ <= fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_less_than_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_less_than_equal(2, 1) == x_less_than_equal);
    FIT_TEST_CHECK(f_less_than_equal(2, 1) == x_less_than_equal);

    const auto x_greater_than_equal = 2 >= 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_greater_than_equal = fit::_ >= fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_greater_than_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_greater_than_equal(2, 1) == x_greater_than_equal);
    FIT_TEST_CHECK(f_greater_than_equal(2, 1) == x_greater_than_equal);

    const auto x_equal = 2 == 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_equal = fit::_ == fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_equal(2, 1) == x_equal);
    FIT_TEST_CHECK(f_equal(2, 1) == x_equal);

    const auto x_not_equal = 2 != 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_not_equal = fit::_ != fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_not_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_not_equal(2, 1) == x_not_equal);
    FIT_TEST_CHECK(f_not_equal(2, 1) == x_not_equal);

    const auto x_bit_and = 2 & 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_bit_and = fit::_ & fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_bit_and)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_bit_and(2, 1) == x_bit_and);
    FIT_TEST_CHECK(f_bit_and(2, 1) == x_bit_and);

    const auto x_xor_ = 2 ^ 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_xor_ = fit::_ ^ fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_xor_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_xor_(2, 1) == x_xor_);
    FIT_TEST_CHECK(f_xor_(2, 1) == x_xor_);

    const auto x_bit_or = 2 | 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_bit_or = fit::_ | fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_bit_or)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_bit_or(2, 1) == x_bit_or);
    FIT_TEST_CHECK(f_bit_or(2, 1) == x_bit_or);

    const auto x_and_ = true && false;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_and_ = fit::_ && fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_and_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_and_(true, false) == x_and_);
    FIT_TEST_CHECK(f_and_(true, false) == x_and_);

    const auto x_or_ = true || false;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_or_ = fit::_ || fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_or_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_or_(true, false) == x_or_);
    FIT_TEST_CHECK(f_or_(true, false) == x_or_);
}

FIT_TEST_CASE()
{
    const auto x_add = 2 + 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_add = 2 + fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_add)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_add(1) == x_add);
    FIT_TEST_CHECK(f_add(1) == x_add);

    const auto x_subtract = 2 - 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_subtract = 2 - fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_subtract)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_subtract(1) == x_subtract);
    FIT_TEST_CHECK(f_subtract(1) == x_subtract);

    const auto x_multiply = 2 * 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_multiply = 2 * fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_multiply)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_multiply(1) == x_multiply);
    FIT_TEST_CHECK(f_multiply(1) == x_multiply);

    const auto x_divide = 2 / 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_divide = 2 / fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_divide)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_divide(1) == x_divide);
    FIT_TEST_CHECK(f_divide(1) == x_divide);

    const auto x_remainder = 2 % 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_remainder = 2 % fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_remainder)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_remainder(1) == x_remainder);
    FIT_TEST_CHECK(f_remainder(1) == x_remainder);

    const auto x_shift_right = 2 >> 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_shift_right = 2 >> fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_shift_right)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_shift_right(1) == x_shift_right);
    FIT_TEST_CHECK(f_shift_right(1) == x_shift_right);

    const auto x_shift_left = 2 << 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_shift_left = 2 << fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_shift_left)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_shift_left(1) == x_shift_left);
    FIT_TEST_CHECK(f_shift_left(1) == x_shift_left);

    const auto x_greater_than = 2 > 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_greater_than = 2 > fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_greater_than)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_greater_than(1) == x_greater_than);
    FIT_TEST_CHECK(f_greater_than(1) == x_greater_than);

    const auto x_less_than = 2 < 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_less_than = 2 < fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_less_than)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_less_than(1) == x_less_than);
    FIT_TEST_CHECK(f_less_than(1) == x_less_than);

    const auto x_less_than_equal = 2 <= 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_less_than_equal = 2 <= fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_less_than_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_less_than_equal(1) == x_less_than_equal);
    FIT_TEST_CHECK(f_less_than_equal(1) == x_less_than_equal);

    const auto x_greater_than_equal = 2 >= 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_greater_than_equal = 2 >= fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_greater_than_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_greater_than_equal(1) == x_greater_than_equal);
    FIT_TEST_CHECK(f_greater_than_equal(1) == x_greater_than_equal);

    const auto x_equal = 2 == 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_equal = 2 == fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_equal(1) == x_equal);
    FIT_TEST_CHECK(f_equal(1) == x_equal);

    const auto x_not_equal = 2 != 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_not_equal = 2 != fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_not_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_not_equal(1) == x_not_equal);
    FIT_TEST_CHECK(f_not_equal(1) == x_not_equal);

    const auto x_bit_and = 2 & 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_bit_and = 2 & fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_bit_and)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_bit_and(1) == x_bit_and);
    FIT_TEST_CHECK(f_bit_and(1) == x_bit_and);

    const auto x_xor_ = 2 ^ 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_xor_ = 2 ^ fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_xor_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_xor_(1) == x_xor_);
    FIT_TEST_CHECK(f_xor_(1) == x_xor_);

    const auto x_bit_or = 2 | 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_bit_or = 2 | fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_bit_or)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_bit_or(1) == x_bit_or);
    FIT_TEST_CHECK(f_bit_or(1) == x_bit_or);

    const auto x_and_ = true && false;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_and_ = true && fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_and_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_and_(false) == x_and_);
    FIT_TEST_CHECK(f_and_(false) == x_and_);

    const auto x_or_ = true || false;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_or_ = true || fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_or_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_or_(false) == x_or_);
    FIT_TEST_CHECK(f_or_(false) == x_or_);
}

FIT_TEST_CASE()
{
    const auto x_add = 2 + 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_add = fit::_ + 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_add)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_add(2) == x_add);
    FIT_TEST_CHECK(f_add(2) == x_add);

    const auto x_subtract = 2 - 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_subtract = fit::_ - 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_subtract)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_subtract(2) == x_subtract);
    FIT_TEST_CHECK(f_subtract(2) == x_subtract);

    const auto x_multiply = 2 * 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_multiply = fit::_ * 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_multiply)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_multiply(2) == x_multiply);
    FIT_TEST_CHECK(f_multiply(2) == x_multiply);

    const auto x_divide = 2 / 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_divide = fit::_ / 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_divide)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_divide(2) == x_divide);
    FIT_TEST_CHECK(f_divide(2) == x_divide);

    const auto x_remainder = 2 % 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_remainder = fit::_ % 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_remainder)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_remainder(2) == x_remainder);
    FIT_TEST_CHECK(f_remainder(2) == x_remainder);

    const auto x_shift_right = 2 >> 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_shift_right = fit::_ >> 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_shift_right)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_shift_right(2) == x_shift_right);
    FIT_TEST_CHECK(f_shift_right(2) == x_shift_right);

    const auto x_shift_left = 2 << 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_shift_left = fit::_ << 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_shift_left)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_shift_left(2) == x_shift_left);
    FIT_TEST_CHECK(f_shift_left(2) == x_shift_left);

    const auto x_greater_than = 2 > 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_greater_than = fit::_ > 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_greater_than)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_greater_than(2) == x_greater_than);
    FIT_TEST_CHECK(f_greater_than(2) == x_greater_than);

    const auto x_less_than = 2 < 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_less_than = fit::_ < 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_less_than)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_less_than(2) == x_less_than);
    FIT_TEST_CHECK(f_less_than(2) == x_less_than);

    const auto x_less_than_equal = 2 <= 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_less_than_equal = fit::_ <= 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_less_than_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_less_than_equal(2) == x_less_than_equal);
    FIT_TEST_CHECK(f_less_than_equal(2) == x_less_than_equal);

    const auto x_greater_than_equal = 2 >= 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_greater_than_equal = fit::_ >= 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_greater_than_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_greater_than_equal(2) == x_greater_than_equal);
    FIT_TEST_CHECK(f_greater_than_equal(2) == x_greater_than_equal);

    const auto x_equal = 2 == 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_equal = fit::_ == 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_equal(2) == x_equal);
    FIT_TEST_CHECK(f_equal(2) == x_equal);

    const auto x_not_equal = 2 != 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_not_equal = fit::_ != 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_not_equal)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_not_equal(2) == x_not_equal);
    FIT_TEST_CHECK(f_not_equal(2) == x_not_equal);

    const auto x_bit_and = 2 & 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_bit_and = fit::_ & 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_bit_and)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_bit_and(2) == x_bit_and);
    FIT_TEST_CHECK(f_bit_and(2) == x_bit_and);

    const auto x_xor_ = 2 ^ 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_xor_ = fit::_ ^ 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_xor_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_xor_(2) == x_xor_);
    FIT_TEST_CHECK(f_xor_(2) == x_xor_);

    const auto x_bit_or = 2 | 1;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_bit_or = fit::_ | 1;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_bit_or)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_bit_or(2) == x_bit_or);
    FIT_TEST_CHECK(f_bit_or(2) == x_bit_or);

    const auto x_and_ = true && false;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_and_ = fit::_ && false;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_and_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_and_(true) == x_and_);
    FIT_TEST_CHECK(f_and_(true) == x_and_);

    const auto x_or_ = true || false;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_or_ = fit::_ || false;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_or_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_or_(true) == x_or_);
    FIT_TEST_CHECK(f_or_(true) == x_or_);
}

FIT_TEST_CASE()
{

    const auto x_not_ = !false;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_not_ = !fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_not_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_not_(false) == x_not_);
    FIT_TEST_CHECK(f_not_(false) == x_not_);

    const auto x_compl_ = ~2;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_compl_ = ~fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_compl_)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_compl_(2) == x_compl_);
    FIT_TEST_CHECK(f_compl_(2) == x_compl_);

    const auto x_unary_plus = +2;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_unary_plus = +fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_unary_plus)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_unary_plus(2) == x_unary_plus);
    FIT_TEST_CHECK(f_unary_plus(2) == x_unary_plus);

    const auto x_unary_subtract = -2;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_unary_subtract = -fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_unary_subtract)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_unary_subtract(2) == x_unary_subtract);
    FIT_TEST_CHECK(f_unary_subtract(2) == x_unary_subtract);

    const auto x_dereference = 2;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_dereference = *fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_dereference)>::value, "Not copyable");
#endif
    FIT_STATIC_TEST_CHECK(f_dereference(&x_dereference) == x_dereference);
    FIT_TEST_CHECK(f_dereference(&x_dereference) == x_dereference);

    // TODO: Test constexpr increment and decrement
#ifndef _MSC_VER
    auto x_increment = 2;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_increment = ++fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_increment)>::value, "Not copyable");
#endif
    f_increment(x_increment);
    FIT_TEST_CHECK(x_increment == 3);

    auto x_decrement = 2;
    FIT_PLACEHOLDER_TEST_CONSTEXPR auto f_decrement = --fit::_;
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ > 6   
    static_assert(std::is_copy_constructible<decltype(f_decrement)>::value, "Not copyable");
#endif
    f_decrement(x_decrement);
    FIT_TEST_CHECK(x_decrement == 1);
#endif

    // TODO: Test post increment and decrement
}

