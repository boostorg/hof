#include <fit/on.h>
#include <fit/placeholders.h>
#include "test.h"

struct foo
{
	constexpr foo(int x) : x(x)
	{}
	int x;
};

struct select_x
{
	template<class T>
	constexpr auto operator()(T&& x) const FIT_RETURNS(x.x);
};

FIT_TEST_CASE()
{
	constexpr auto add = fit::_ + fit::_;
	static_assert(fit::on(select_x(), fit::_ + fit::_)(foo(1), foo(2)) == 3, "Constexpr projection failed");
	FIT_TEST_CHECK(fit::on(std::mem_fn(&foo::x), fit::_ + fit::_)(foo(1), foo(2)) == 3);
}
