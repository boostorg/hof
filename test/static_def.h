
#ifndef GUARD_STATIC_DEF
#define GUARD_STATIC_DEF

#include <fit/function.h>

namespace fit_test {

FIT_STATIC_FUNCTION(fit_sum_lambda) = [](int x, int y) 
{
    return x + y;
};

struct fit_sum_f
{
    int operator()(int x, int y) const
    {
        return x + y;
    }
};

FIT_STATIC_FUNCTION(fit_sum_fo) = fit_sum_f();

// FIT_STATIC_FUNCTION(fit_sum) = [](auto x, auto y) 
// {
//     return x + y;
// };

template<class T>
T fit_sum(T x, T y) 
{
    return x + y;
};

}

#endif
