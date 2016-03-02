
#include "test.hpp"
#include <vector>
#include <boost/fit/pipable.hpp>
#include <boost/fit/placeholders.hpp>
#include <algorithm>


struct filter_fn
{
    template<class Input, class P>
    Input operator()(Input input, P pred) const
    {
        Input output(input.size());
        output.erase(
            ::std::copy_if(
                ::std::begin(input),
                ::std::end(input),
                ::std::begin(output),
                pred
            ),
            ::std::end(output)
        );
        return output;
    }
};

static constexpr boost::fit::pipable_adaptor<filter_fn> filter = {};

BOOST_FIT_TEST_CASE()
{
    std::vector<int> data;
    for(int i=0;i<6;i++)
    {
        data.push_back(i);
    }
    std::vector<int> r1 = data | filter(boost::fit::_1 > 1);
    BOOST_FIT_TEST_CHECK(r1.size() == 4);

    std::vector<int> r2 = filter(data, boost::fit::_1 > 1);
    BOOST_FIT_TEST_CHECK(r2.size() == 4);
}
