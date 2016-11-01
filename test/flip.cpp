#include <fit/flip.hpp>
#include <fit/placeholders.hpp>
#include "test.hpp"

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::flip(fit::_ - fit::_)(2, 5));
    FIT_STATIC_TEST_CHECK(3 == fit::flip(fit::_ - fit::_)(2, 5));
}

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
