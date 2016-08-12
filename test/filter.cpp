#include <fit/if.hpp>
#include "test.hpp"

#include <fit/by.hpp>
#include <fit/lift.hpp>
#include <fit/construct.hpp>
#include <fit/conditional.hpp>
#include <fit/unpack.hpp>

#include <tuple>

#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8)
#define FIT_HAS_CONSTEXPR_TUPLE 0
#else
#define FIT_HAS_CONSTEXPR_TUPLE 1
#endif


FIT_LIFT_CLASS(make_tuple_f, std::make_tuple);

struct integer_predicate
{
    constexpr integer_predicate()
    {}
    template<class T>
    constexpr auto operator()(T x) const FIT_RETURNS
    (
        fit::conditional(
            fit::if_(std::is_integral<T>())(fit::pack_basic),
            fit::always(fit::pack_basic())
        )(fit::move(x))
    )
};

struct filter_integers
{
    template<class Seq>
    constexpr auto operator()(Seq s) const FIT_RETURNS
    (
        fit::unpack(
            fit::by(integer_predicate(), fit::unpack(make_tuple_f()))
        )(std::move(s))
    )
};


FIT_TEST_CASE()
{
    FIT_TEST_CHECK(filter_integers()(fit::pack_basic(1, 2, 2.0, 3)) == std::make_tuple(1, 2, 3));
#if FIT_HAS_CONSTEXPR_TUPLE
    FIT_STATIC_TEST_CHECK(filter_integers()(fit::pack_basic(1, 2, 2.0, 3)) == std::make_tuple(1, 2, 3));
#endif
}


