
#include "static_def.h"

extern int f();

int main()
{
    if (fit_test::fit_sum(1, 2) != 3) printf("FAILED\n");
    printf("PASSED\n");
    return f();
}
