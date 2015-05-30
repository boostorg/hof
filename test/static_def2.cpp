
#include <cstdio>
#include "static_def.h"

extern void* f_sum_lambda_addr();
extern void* f_sum_fo_addr();

extern void* sum_lambda_addr();
extern void* sum_fo_addr();

extern void* f_sum_var_addr();
extern void* f_sum_constexpr_fo_addr();

extern void* sum_var_addr();
extern void* sum_constexpr_fo_addr();

void* f_sum_lambda_addr()
{
    return (void*)&fit_test::fit_sum_lambda;
}
void* f_sum_fo_addr()
{
    return (void*)&fit_test::fit_sum_fo;
}

void* f_sum_var_addr()
{
    return (void*)&fit_test::fit_sum_var;
}
void* f_sum_constexpr_fo_addr()
{
    return (void*)&fit_test::fit_sum_constexpr_fo;
}

int f()
{
    if (fit_test::fit_sum_fo(1, 2) != 3) printf("FAILED\n");
    if (fit_test::fit_sum_lambda(1, 2) != 3) printf("FAILED\n");
    if (fit_test::fit_sum(1, 2) != 3) printf("FAILED\n");

#if !FIT_NO_UNIQUE_STATIC_LAMBDA_FUNCTION_ADDR
    if (sum_lambda_addr() != f_sum_lambda_addr()) printf("FAILED: Lambda\n");
    if (sum_fo_addr() != f_sum_fo_addr()) printf("FAILED: Function object\n");
#endif

#if !FIT_NO_UNIQUE_STATIC_VAR
    if (sum_var_addr() != f_sum_var_addr()) printf("FAILED: Lambda\n");
    if (sum_constexpr_fo_addr() != f_sum_constexpr_fo_addr()) printf("FAILED: Function object\n");
#endif
    return 0;
}

