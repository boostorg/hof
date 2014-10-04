#include <fit/invoke.h>
#include "test.h"

struct my_add
{
	template<class T>
	constexpr T operator()(T x, T y) const
	{
		return x + y;
	}
};

FIT_TEST_CASE()
{
    std::plus<int> add;
    FIT_TEST_CHECK(fit::invoke(add,std::make_tuple(1,1)) == 2);
    FIT_STATIC_TEST_CHECK(fit::invoke(my_add(), std::make_tuple(1,1)) == 2);
}