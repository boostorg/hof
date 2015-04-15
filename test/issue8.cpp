
#include "test.h"
#include <vector>
#include <fit/pipable.h>
#include <fit/placeholders.h>
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

static constexpr fit::pipable_adaptor<filter_fn> filter = {};

FIT_TEST_CASE()
{
    std::vector<int> data;
    for(int i=0;i<6;i++)
    {
        data.push_back(i);
    }
    std::vector<int> r1 = data | filter(fit::_1 > 1);
    FIT_TEST_CHECK(r1.size() == 4);

    std::vector<int> r2 = filter(data, fit::_1 > 1);
    FIT_TEST_CHECK(r2.size() == 4);
}
