#include <boost/fit/always.hpp>
#include <memory>
#include "test.hpp"

BOOST_FIT_TEST_CASE()
{
    static const int ten = 10;
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::always(ten)(1,2,3,4,5) == 10);
    BOOST_FIT_TEST_CHECK( boost::fit::always(ten)(1,2,3,4,5) == 10 );
    
    int i = 10; 
    BOOST_FIT_TEST_CHECK( boost::fit::always(std::ref(i))(1,2,3,4,5) == 10 );
    BOOST_FIT_TEST_CHECK( &boost::fit::always(std::ref(i))(1,2,3,4,5) == &i );

    boost::fit::always()(1, 2);
    static_assert(std::is_same<decltype(boost::fit::always()(1, 2)), BOOST_FIT_ALWAYS_VOID_RETURN>::value, "Failed");
}

