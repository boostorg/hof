#include <fit/always.h>
#include <memory>
#include "test.h"

FIT_TEST_CASE()
{
    static const int ten = 10;
    FIT_STATIC_TEST_CHECK(fit::always(ten)(1,2,3,4,5) == 10);
    FIT_TEST_CHECK( fit::always(ten)(1,2,3,4,5) == 10 );
    
    int i = 10; 
    FIT_TEST_CHECK( fit::always(std::ref(i))(1,2,3,4,5) == 10 );
    FIT_TEST_CHECK( &fit::always(std::ref(i))(1,2,3,4,5) == &i );
}

