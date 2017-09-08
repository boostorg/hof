#include <boost/fit/flip.hpp>
#include <boost/fit/by.hpp>
#include <boost/fit/construct.hpp>
#include <boost/fit/pipable.hpp>
#include <boost/fit/rotate.hpp>
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

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::flip(derived())(nullptr, 2) == 2);
    BOOST_FIT_TEST_CHECK(boost::fit::rotate(derived())(nullptr, 2) == 2);
    BOOST_FIT_TEST_CHECK((2 | boost::fit::pipable(derived())(nullptr)) == 2);
}
