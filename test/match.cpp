#include <fit/match.h>
#include <fit/static.h>
#include "test.h"

FIT_STATIC_TEST_CASE()
{
	struct int_class
	{
	    int operator()(int) const
	    {
	        return 1;
	    }
	};

	struct foo
	{};

	struct foo_class
	{
	    foo operator()(foo) const
	    {
	        return foo();
	    }
	};

	fit::static_<fit::match_adaptor<int_class, foo_class> > fun = {};

	static_assert(std::is_same<int, decltype(fun(1))>::value, "Failed match");
	static_assert(std::is_same<foo, decltype(fun(foo()))>::value, "Failed match");
};

// TODO: Test constexpr