#include <boost/fit/apply_eval.hpp>
#include <boost/fit/always.hpp>
#include "test.hpp"

#include <memory>

BOOST_FIT_TEST_CASE()
{    
    BOOST_FIT_STATIC_TEST_CHECK(boost::fit::apply_eval(binary_class(), boost::fit::always(1), boost::fit::always(2)) == 3);
    BOOST_FIT_TEST_CHECK(boost::fit::apply_eval(binary_class(), []{ return 1; }, []{ return 2;}) == 3);
}

BOOST_FIT_TEST_CASE()
{
    boost::fit::apply_eval(boost::fit::always(), boost::fit::always(1), boost::fit::always(2));
}

struct indirect_sum_f
{
    template<class T, class U>
    auto operator()(T x, U y) const
    BOOST_FIT_RETURNS(*x + *y);
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(
        boost::fit::apply_eval(
            indirect_sum_f(), 
            []{ return std::unique_ptr<int>(new int(1)); }, 
            []{ return std::unique_ptr<int>(new int(2)); })
        == 3);
}
