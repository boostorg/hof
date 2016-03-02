
#ifndef GUARD_STATIC_DEF
#define GUARD_STATIC_DEF

#include <boost/fit/function.hpp>
#include <boost/fit/lambda.hpp>

namespace fit_test {
#if BOOST_FIT_HAS_STATIC_LAMBDA
BOOST_FIT_STATIC_LAMBDA_FUNCTION(fit_sum_lambda) = [](int x, int y) 
{
    return x + y;
};
#endif

struct fit_sum_f
{
    constexpr int operator()(int x, int y) const
    {
        return x + y;
    }
};
#if BOOST_FIT_HAS_STATIC_LAMBDA
BOOST_FIT_STATIC_LAMBDA_FUNCTION(fit_sum_fo) = fit_sum_f();
#endif
BOOST_FIT_STATIC_FUNCTION(fit_sum_constexpr_fo) = fit_sum_f();

BOOST_FIT_DECLARE_STATIC_VAR(fit_sum_var, fit_sum_f);

// BOOST_FIT_STATIC_FUNCTION(fit_sum) = [](auto x, auto y) 
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
