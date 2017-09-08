#include <boost/fit/returns.hpp>
#include "test.hpp"

#if !defined (__GNUC__) || defined (__clang__)
struct add_1
{
    int a;
    add_1() : a(1) {}
    
    BOOST_FIT_RETURNS_CLASS(add_1);
    
    template<class T>
    auto operator()(T x) const 
    BOOST_FIT_RETURNS(x+BOOST_FIT_CONST_THIS->a);
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(3 == add_1()(2));   
}
#endif
