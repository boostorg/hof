#include <fit/apply_eval.h>
#include <fit/always.h>
#include "test.h"

#include <memory>

FIT_TEST_CASE()
{    
    FIT_STATIC_TEST_CHECK(fit::apply_eval(binary_class(), fit::always(1), fit::always(2)) == 3);
    FIT_TEST_CHECK(fit::apply_eval(binary_class(), []{ return 1; }, []{ return 2;}) == 3);
}

FIT_TEST_CASE()
{
    fit::apply_eval(fit::always(), fit::always(1), fit::always(2));
}

struct indirect_sum_f
{
    template<class T, class U>
    auto operator()(T x, U y) const
    FIT_RETURNS(*x + *y);
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(
        fit::apply_eval(
            indirect_sum_f(), 
            []{ return std::unique_ptr<int>(new int(1)); }, 
            []{ return std::unique_ptr<int>(new int(2)); })
        == 3);
}
