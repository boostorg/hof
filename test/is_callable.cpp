#include <fit/is_callable.h>
#include <ciso646>
#include "test.h"


FIT_STATIC_TEST_CASE()
{
    struct is_callable_class
    {
        void operator()(int) const
        {
        }
    };
    struct callable_test_param {};

    void is_callable_function(int)
    {
    }

    static_assert(fit::is_callable<is_callable_class, int>::value, "Not callable");
    static_assert(fit::is_callable<is_callable_class, const int&>::value, "Not callable");
    static_assert(not fit::is_callable<is_callable_class, callable_test_param>::value, "callable failed");
    static_assert(not fit::is_callable<is_callable_class>::value, "callable failed");
    static_assert(not fit::is_callable<is_callable_class, int, int>::value, "callable failed");

    typedef void (*is_callable_function_pointer)(int);
    static_assert(fit::is_callable<is_callable_function_pointer, int>::value, "Not callable");
    static_assert(fit::is_callable<is_callable_function_pointer, const int&>::value, "Not callable");
    static_assert(not fit::is_callable<is_callable_function_pointer, callable_test_param>::value, "callable failed");
    static_assert(not fit::is_callable<is_callable_function_pointer>::value, "callable failed");
    static_assert(not fit::is_callable<is_callable_function_pointer, int, int>::value, "callable failed");
};
