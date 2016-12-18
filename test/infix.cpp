#include <fit/infix.hpp>
#include <fit/function.hpp>
#include <fit/lambda.hpp>
#include <fit/placeholders.hpp>
#include "test.hpp"

struct sum_f
{
    template<class T, class U>
    constexpr T operator()(T x, U y) const FIT_RETURNS_DEDUCE_NOEXCEPT(x+y)
    {
        return x+y;
    }
};

static constexpr fit::infix_adaptor<sum_f> sum = {};

#if FIT_HAS_NOEXCEPT_DEDUCTION
FIT_TEST_CASE()
{
    static_assert(noexcept(1 <sum> 2), "noexcept infix");
    static_assert(!noexcept(std::string() <sum> std::string()), "noexcept infix");
}
#endif

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == (1 <sum> 2));
    FIT_STATIC_TEST_CHECK(3 == (1 <sum> 2));
    
    FIT_TEST_CHECK(3 == (sum(1, 2)));
    FIT_STATIC_TEST_CHECK(3 == (sum(1, 2)));
}

FIT_STATIC_FUNCTION(sum1) = fit::infix(sum_f());

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == (1 <sum1> 2));
    FIT_STATIC_TEST_CHECK(3 == (1 <sum1> 2));

    FIT_TEST_CHECK(3 == (sum1(1, 2)));
    FIT_STATIC_TEST_CHECK(3 == (sum1(1, 2)));
}

FIT_STATIC_LAMBDA_FUNCTION(sum2) = fit::infix([](int x, int y) { return x + y; });

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == (1 <sum2> 2));

    FIT_TEST_CHECK(3 == (sum2(1, 2)));
}

FIT_STATIC_FUNCTION(sum3) = fit::infix(fit::_ + fit::_);

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == (1 <sum3> 2));
    FIT_STATIC_TEST_CHECK(3 == (1 <sum3> 2));

    FIT_TEST_CHECK(3 == (sum3(1, 2)));
    FIT_STATIC_TEST_CHECK(3 == (sum3(1, 2)));
}


FIT_STATIC_LAMBDA_FUNCTION(sum4) = fit::infix(fit::infix([](int x, int y) { return x + y; }));

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == (1 <sum4> 2));

    FIT_TEST_CHECK(3 == (sum4(1, 2)));
}

FIT_STATIC_FUNCTION(sum5) = fit::infix(fit::infix(fit::_ + fit::_));

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == (1 <sum5> 2));
    FIT_STATIC_TEST_CHECK(3 == (1 <sum5> 2));

    FIT_TEST_CHECK(3 == (sum5(1, 2)));
    FIT_STATIC_TEST_CHECK(3 == (sum5(1, 2)));
}
