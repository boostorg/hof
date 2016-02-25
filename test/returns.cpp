#include <fit/returns.hpp>
#include "test.hpp"

#if !defined (__GNUC__) || defined (__clang__)
struct add_1
{
    int a;
    add_1() : a(1) {}
    
    FIT_RETURNS_CLASS(add_1);
    
    template<class T>
    auto operator()(T x) const 
    FIT_RETURNS(x+FIT_CONST_THIS->a);
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == add_1()(2));   
}
#endif
