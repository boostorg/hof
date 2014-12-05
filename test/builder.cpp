#include <fit/builder.h>
#include <memory>
#include "test.h"

template<template<class...> class Trait, class... Ts>
struct as_integral_constant
{
    typedef Trait<typename std::remove_reference<Ts>::type...> result_type;
    typedef std::integral_constant<typename result_type::value_type, result_type::value> type;
};

template<template<class...> class Trait, class... Ts>
constexpr typename as_integral_constant<Trait, Ts...>::type
trait(Ts&&...)
{
    return typename as_integral_constant<Trait, Ts...>::type();
}

template<class... Ts>
struct is_integral
: fit::detail::and_<std::is_integral<Ts>...>
{};

namespace builder {

// class
FIT_FUNCTION_CLASS((sum_class)(const auto x, const auto y) if(trait<is_integral>(x, y))(x + y) else (1))
// object
FIT_FUNCTION_OBJECT((sum)(const auto x, const auto y) if(trait<is_integral>(x, y))(x + y) else (1))
FIT_FUNCTION_OBJECT((sum2)(const auto x, const int y) if(trait<is_integral>(x))(x + y))
// pipe
FIT_FUNCTION_PIPE_OBJECT((sum_pipe)(const auto x, const auto y) if(trait<is_integral>(x, y))(x + y) else (1))

// // const_value
// FIT_FUNCTION_OBJECT((const_value)(const auto x) if(x == std::integral_constant<int, 1>())(true))
// FIT_FUNCTION_OBJECT((const_value)(const auto x) if(x == std::integral_constant<int, 1>())(true) else (false))
// FIT_FUNCTION_OBJECT((const_value2)(const auto x) if(x == std::integral_constant<int, 1>())(true) else if(x == std::integral_constant<int, 2>())(false))

}

struct foo {};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK( builder::sum(1, 2) == 3 );
    FIT_TEST_CHECK( builder::sum2(1, 2) == 3 );
    FIT_TEST_CHECK( builder::sum_class()(1, 2) == 3 );
    FIT_TEST_CHECK( (1 | builder::sum_pipe(2)) == 3 );
    FIT_TEST_CHECK( builder::sum(foo(), 2) == 1 );
    // FIT_TEST_CHECK( const_value(std::integral_constant<int, 1>()) );
    // FIT_TEST_CHECK( !const_value(std::integral_constant<int, 0>()) );

}

