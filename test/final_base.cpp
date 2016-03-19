#include <fit/flip.hpp>
#include "test.hpp"

#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define FINAL
#else
#define FINAL final
#endif


struct f FINAL {
    int operator()(int i, void *) const {
        return i;
    }
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::flip(f())(nullptr, 2) == 2);
}
