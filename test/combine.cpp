#include <fit/combine.h>
#include "test.h"

#include <fit/construct.h>
#include <fit/capture.h>
#include <utility>
#include <tuple>

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(
        fit::combine(
            fit::construct<std::tuple>(),
            fit::capture(1)(fit::construct<std::pair>()),
            fit::capture(2)(fit::construct<std::pair>())
        )(2, 4) 
        == std::make_tuple(std::make_pair(1, 2), std::make_pair(2, 4)));
}
