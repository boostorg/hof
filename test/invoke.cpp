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
    static_assert(fit::invoke(my_add(), std::make_tuple(1,1)) == 2, "my_add constexpr failed");
}