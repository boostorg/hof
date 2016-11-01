#include <fit/flip.hpp>
#include <fit/by.hpp>
#include <fit/construct.hpp>
#include <fit/pipable.hpp>
#include "test.hpp"

struct base { 
    base(int) {} 
    base(const base&) {}
    virtual ~base();
};

base::~base() {}

struct derived : virtual base {
    derived() : base(1) {}
    derived(const derived&) : base(1) {}
    int operator()(int i, void *) const {
        return i;
    }
    ~derived();
};
derived::~derived() {}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::flip(derived())(nullptr, 2) == 2);
    FIT_TEST_CHECK((2 | fit::pipable(derived())(nullptr)) == 2);
}
