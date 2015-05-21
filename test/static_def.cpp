
#include <cstdio>
#include "static_def.h"

extern int f();

extern void* f_sum_lambda_addr();
extern void* f_sum_fo_addr();

extern void* sum_lambda_addr();
extern void* sum_fo_addr();

void* sum_lambda_addr()
{
    return (void*)&fit_test::fit_sum_lambda;
}
void* sum_fo_addr()
{
    return (void*)&fit_test::fit_sum_fo;
}

int main()
{
    if (fit_test::fit_sum_fo(1, 2) != 3) printf("FAILED\n");
    if (fit_test::fit_sum_lambda(1, 2) != 3) printf("FAILED\n");
    if (fit_test::fit_sum(1, 2) != 3) printf("FAILED\n");

#if !FIT_NO_STATIC_CONST
    if (sum_lambda_addr() != f_sum_lambda_addr()) printf("FAILED: Lambda\n");
    if (sum_fo_addr() != f_sum_fo_addr()) printf("FAILED: Function object\n");
#endif
    return f();
}
