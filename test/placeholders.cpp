#include <fit/placeholders.h>
#include "test.h"

// TODO: Test assign operators

struct square
{
    template<class T>
    constexpr auto operator()(T x) const FIT_RETURNS(x*x);
};

FIT_TEST_CASE()
{
    const auto x_square_add = 2 + (4*4);
    constexpr auto f_square_add = fit::_1 + fit::lazy(square())(fit::_2);
    static_assert(f_square_add(2, 4) == x_square_add, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_square_add(2, 4) == x_square_add);
}

FIT_TEST_CASE()
{
    const auto x_add = 2 + 1;
    constexpr auto f_add = fit::_1 + fit::_2;
    static_assert(f_add(2, 1) == x_add, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_add(2, 1) == x_add);

    const auto x_subtract = 2 - 1;
    constexpr auto f_subtract = fit::_1 - fit::_2;
    static_assert(f_subtract(2, 1) == x_subtract, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_subtract(2, 1) == x_subtract);

    const auto x_multiply = 2 * 1;
    constexpr auto f_multiply = fit::_1 * fit::_2;
    static_assert(f_multiply(2, 1) == x_multiply, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_multiply(2, 1) == x_multiply);

    const auto x_divide = 2 / 1;
    constexpr auto f_divide = fit::_1 / fit::_2;
    static_assert(f_divide(2, 1) == x_divide, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_divide(2, 1) == x_divide);

    const auto x_remainder = 2 % 1;
    constexpr auto f_remainder = fit::_1 % fit::_2;
    static_assert(f_remainder(2, 1) == x_remainder,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_remainder(2, 1) == x_remainder);

    const auto x_shift_right = 2 >> 1;
    constexpr auto f_shift_right = fit::_1 >> fit::_2;
    static_assert(f_shift_right(2, 1) == x_shift_right,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_shift_right(2, 1) == x_shift_right);

    const auto x_shift_left = 2 << 1;
    constexpr auto f_shift_left = fit::_1 << fit::_2;
    static_assert(f_shift_left(2, 1) == x_shift_left,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_shift_left(2, 1) == x_shift_left);

    const auto x_greater_than = 2 > 1;
    constexpr auto f_greater_than = fit::_1 > fit::_2;
    static_assert(f_greater_than(2, 1) == x_greater_than,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_greater_than(2, 1) == x_greater_than);

    const auto x_less_than = 2 < 1;
    constexpr auto f_less_than = fit::_1 < fit::_2;
    static_assert(f_less_than(2, 1) == x_less_than,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_less_than(2, 1) == x_less_than);

    const auto x_less_than_equal = 2 <= 1;
    constexpr auto f_less_than_equal = fit::_1 <= fit::_2;
    static_assert(f_less_than_equal(2, 1) == x_less_than_equal,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_less_than_equal(2, 1) == x_less_than_equal);

    const auto x_greater_than_equal = 2 >= 1;
    constexpr auto f_greater_than_equal = fit::_1 >= fit::_2;
    static_assert(f_greater_than_equal(2, 1) == x_greater_than_equal,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_greater_than_equal(2, 1) == x_greater_than_equal);

    const auto x_equal = 2 == 1;
    constexpr auto f_equal = fit::_1 == fit::_2;
    static_assert(f_equal(2, 1) == x_equal, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_equal(2, 1) == x_equal);

    const auto x_not_equal = 2 != 1;
    constexpr auto f_not_equal = fit::_1 != fit::_2;
    static_assert(f_not_equal(2, 1) == x_not_equal,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_not_equal(2, 1) == x_not_equal);

    const auto x_bit_and = 2 & 1;
    constexpr auto f_bit_and = fit::_1 & fit::_2;
    static_assert(f_bit_and(2, 1) == x_bit_and, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_bit_and(2, 1) == x_bit_and);

    const auto x_xor_ = 2 ^ 1;
    constexpr auto f_xor_ = fit::_1 ^ fit::_2;
    static_assert(f_xor_(2, 1) == x_xor_, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_xor_(2, 1) == x_xor_);

    const auto x_bit_or = 2 | 1;
    constexpr auto f_bit_or = fit::_1 | fit::_2;
    static_assert(f_bit_or(2, 1) == x_bit_or, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_bit_or(2, 1) == x_bit_or);

    const auto x_and_ = true && false;
    constexpr auto f_and_ = fit::_1 && fit::_2;
    static_assert(f_and_(true, false) == x_and_, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_and_(true, false) == x_and_);

    const auto x_or_ = true || false;
    constexpr auto f_or_ = fit::_1 || fit::_2;
    static_assert(f_or_(true, false) == x_or_, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_or_(true, false) == x_or_);
}

FIT_TEST_CASE()
{

    const auto x_not_ = !false;
    constexpr auto f_not_ = !fit::_1;
    static_assert(f_not_(false) == x_not_, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_not_(false) == x_not_);

    const auto x_compl_ = ~2;
    constexpr auto f_compl_ = ~fit::_1;
    static_assert(f_compl_(2) == x_compl_, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_compl_(2) == x_compl_);

    const auto x_unary_plus = +2;
    constexpr auto f_unary_plus = +fit::_1;
    static_assert(f_unary_plus(2) == x_unary_plus,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_unary_plus(2) == x_unary_plus);

    const auto x_unary_subtract = -2;
    constexpr auto f_unary_subtract = -fit::_1;
    static_assert(f_unary_subtract(2) == x_unary_subtract,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_unary_subtract(2) == x_unary_subtract);

    const auto x_dereference = 2;
    constexpr auto f_dereference = *fit::_1;
    static_assert(f_dereference(&x_dereference) == x_dereference,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_dereference(&x_dereference) == x_dereference);

    // TODO: Test constexpr increment and decrement

    auto x_increment = 2;
    constexpr auto f_increment = ++fit::_1;
    f_increment(x_increment);
    FIT_TEST_CHECK(x_increment == 3);

    auto x_decrement = 2;
    constexpr auto f_decrement = --fit::_1;
    f_decrement(x_decrement);
    FIT_TEST_CHECK(x_decrement == 1);

    // TODO: Test post increment and decrement
}


FIT_TEST_CASE()
{
    const auto x_add = 2 + 1;
    constexpr auto f_add = fit::_ + fit::_;
    static_assert(f_add(2, 1) == x_add, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_add(2, 1) == x_add);

    const auto x_subtract = 2 - 1;
    constexpr auto f_subtract = fit::_ - fit::_;
    static_assert(f_subtract(2, 1) == x_subtract, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_subtract(2, 1) == x_subtract);

    const auto x_multiply = 2 * 1;
    constexpr auto f_multiply = fit::_ * fit::_;
    static_assert(f_multiply(2, 1) == x_multiply, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_multiply(2, 1) == x_multiply);

    const auto x_divide = 2 / 1;
    constexpr auto f_divide = fit::_ / fit::_;
    static_assert(f_divide(2, 1) == x_divide, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_divide(2, 1) == x_divide);

    const auto x_remainder = 2 % 1;
    constexpr auto f_remainder = fit::_ % fit::_;
    static_assert(f_remainder(2, 1) == x_remainder,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_remainder(2, 1) == x_remainder);

    const auto x_shift_right = 2 >> 1;
    constexpr auto f_shift_right = fit::_ >> fit::_;
    static_assert(f_shift_right(2, 1) == x_shift_right,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_shift_right(2, 1) == x_shift_right);

    const auto x_shift_left = 2 << 1;
    constexpr auto f_shift_left = fit::_ << fit::_;
    static_assert(f_shift_left(2, 1) == x_shift_left,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_shift_left(2, 1) == x_shift_left);

    const auto x_greater_than = 2 > 1;
    constexpr auto f_greater_than = fit::_ > fit::_;
    static_assert(f_greater_than(2, 1) == x_greater_than,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_greater_than(2, 1) == x_greater_than);

    const auto x_less_than = 2 < 1;
    constexpr auto f_less_than = fit::_ < fit::_;
    static_assert(f_less_than(2, 1) == x_less_than,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_less_than(2, 1) == x_less_than);

    const auto x_less_than_equal = 2 <= 1;
    constexpr auto f_less_than_equal = fit::_ <= fit::_;
    static_assert(f_less_than_equal(2, 1) == x_less_than_equal,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_less_than_equal(2, 1) == x_less_than_equal);

    const auto x_greater_than_equal = 2 >= 1;
    constexpr auto f_greater_than_equal = fit::_ >= fit::_;
    static_assert(f_greater_than_equal(2, 1) == x_greater_than_equal,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_greater_than_equal(2, 1) == x_greater_than_equal);

    const auto x_equal = 2 == 1;
    constexpr auto f_equal = fit::_ == fit::_;
    static_assert(f_equal(2, 1) == x_equal, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_equal(2, 1) == x_equal);

    const auto x_not_equal = 2 != 1;
    constexpr auto f_not_equal = fit::_ != fit::_;
    static_assert(f_not_equal(2, 1) == x_not_equal,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_not_equal(2, 1) == x_not_equal);

    const auto x_bit_and = 2 & 1;
    constexpr auto f_bit_and = fit::_ & fit::_;
    static_assert(f_bit_and(2, 1) == x_bit_and, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_bit_and(2, 1) == x_bit_and);

    const auto x_xor_ = 2 ^ 1;
    constexpr auto f_xor_ = fit::_ ^ fit::_;
    static_assert(f_xor_(2, 1) == x_xor_, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_xor_(2, 1) == x_xor_);

    const auto x_bit_or = 2 | 1;
    constexpr auto f_bit_or = fit::_ | fit::_;
    static_assert(f_bit_or(2, 1) == x_bit_or, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_bit_or(2, 1) == x_bit_or);

    const auto x_and_ = true && false;
    constexpr auto f_and_ = fit::_ && fit::_;
    static_assert(f_and_(true, false) == x_and_, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_and_(true, false) == x_and_);

    const auto x_or_ = true || false;
    constexpr auto f_or_ = fit::_ || fit::_;
    static_assert(f_or_(true, false) == x_or_, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_or_(true, false) == x_or_);
}

FIT_TEST_CASE()
{

    const auto x_not_ = !false;
    constexpr auto f_not_ = !fit::_;
    static_assert(f_not_(false) == x_not_, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_not_(false) == x_not_);

    const auto x_compl_ = ~2;
    constexpr auto f_compl_ = ~fit::_;
    static_assert(f_compl_(2) == x_compl_, "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_compl_(2) == x_compl_);

    const auto x_unary_plus = +2;
    constexpr auto f_unary_plus = +fit::_;
    static_assert(f_unary_plus(2) == x_unary_plus,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_unary_plus(2) == x_unary_plus);

    const auto x_unary_subtract = -2;
    constexpr auto f_unary_subtract = -fit::_;
    static_assert(f_unary_subtract(2) == x_unary_subtract,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_unary_subtract(2) == x_unary_subtract);

    const auto x_dereference = 2;
    constexpr auto f_dereference = *fit::_;
    static_assert(f_dereference(&x_dereference) == x_dereference,
                "Constexpr placeholder failed");
    FIT_TEST_CHECK(f_dereference(&x_dereference) == x_dereference);

    // TODO: Test constexpr increment and decrement

    auto x_increment = 2;
    constexpr auto f_increment = ++fit::_;
    f_increment(x_increment);
    FIT_TEST_CHECK(x_increment == 3);

    auto x_decrement = 2;
    constexpr auto f_decrement = --fit::_;
    f_decrement(x_decrement);
    FIT_TEST_CHECK(x_decrement == 1);

    // TODO: Test post increment and decrement
}

