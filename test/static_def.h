
#ifndef GUARD_STATIC_DEF
#define GUARD_STATIC_DEF

#include <fit/function.h>

namespace fit_test {

FIT_STATIC_FUNCTION(fit_sum) = [](int x, int y) 
{
    return x + y;
};

// FIT_STATIC_FUNCTION(fit_sum) = [](auto x, auto y) 
// {
//     return x + y;
// };

// template<class T>
// T fit_sum(T x, T y) 
// {
//     return x + y;
// };

}

#endif
