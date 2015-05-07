#include <fit/apply_eval.h>
#include "test.h"

FIT_TEST_CASE()
{    
    FIT_STATIC_TEST_CHECK(fit::apply_eval(binary_class(), fit::always(1), fit::always(2)) == 3);
    FIT_TEST_CHECK(fit::apply_eval(binary_class(), []{ return 1; }, []{ return 2;}) == 3);
}
