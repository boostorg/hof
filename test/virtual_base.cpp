#include <fit/flip.hpp>
#include "test.hpp"

struct base { 
    base(int) {} 
    virtual ~base() {}
};

struct derived : virtual base {
    derived() : base(1) {}
    int operator()(int i, void *) const {
        return i;
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::flip(derived())(nullptr, 2) == 2);
}
