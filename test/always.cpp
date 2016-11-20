#include <fit/always.hpp>
#include <fit/function.hpp>
#include <memory>
#include "test.hpp"

FIT_TEST_CASE()
{
    static const int ten = 10;
    FIT_STATIC_TEST_CHECK(fit::always(ten)(1,2,3,4,5) == 10);
    FIT_TEST_CHECK( fit::always(ten)(1,2,3,4,5) == 10 );
    
    int i = 10; 
    FIT_TEST_CHECK( fit::always(std::ref(i))(1,2,3,4,5) == 10 );
    FIT_TEST_CHECK( &fit::always(std::ref(i))(1,2,3,4,5) == &i );

    fit::always()(1, 2);
    static_assert(std::is_same<decltype(fit::always()(1, 2)), FIT_ALWAYS_VOID_RETURN>::value, "Failed");
}

FIT_TEST_CASE()
{
    int i = 10; 
    FIT_TEST_CHECK( fit::always_ref(i)(1,2,3,4,5) == 10 );
    FIT_TEST_CHECK( &fit::always_ref(i)(1,2,3,4,5) == &i );
}

FIT_STATIC_FUNCTION(gten) = fit::always(std::integral_constant<int, 10>{});

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(gten(1,2,3,4,5) == 10);
    FIT_TEST_CHECK(gten(1,2,3,4,5) == 10);
}

FIT_TEST_CASE()
{
    auto f = fit::always(10);
    STATIC_ASSERT_NOT_DEFAULT_CONSTRUCTIBLE(decltype(f));
    FIT_TEST_CHECK(f(1,2,3,4,5) == 10);
}
