#include <fit/implicit.h>
#include "test.h"

template<class T>
struct auto_caster
{
    template<class U>
    T operator()(U x)
    {
        return T(x);
    }
};

struct auto_caster_foo
{
    int i;
    explicit auto_caster_foo(int i) : i(i) {}

};
// TODO: Test template constraint on conversion operator
fit::implicit<auto_caster> auto_cast = {};

FIT_TEST_CASE()
{
    float f = 1.5;
    int i = auto_cast(f);
    // auto_caster_foo x = 1;
    auto_caster_foo x = auto_cast(1);
    FIT_TEST_CHECK(1 == i);
    FIT_TEST_CHECK(1 == x.i);

}