#include <boost/fit/if.hpp>
#include "test.hpp"

#include <boost/fit/by.hpp>
#include <boost/fit/lift.hpp>
#include <boost/fit/construct.hpp>
#include <boost/fit/conditional.hpp>
#include <boost/fit/unpack.hpp>

#include <tuple>

#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8)
#define BOOST_FIT_HAS_CONSTEXPR_TUPLE 0
#else
#define BOOST_FIT_HAS_CONSTEXPR_TUPLE 1
#endif


BOOST_FIT_LIFT_CLASS(make_tuple_f, std::make_tuple);

struct integer_predicate
{
    constexpr integer_predicate()
    {}
    template<class T>
    constexpr auto operator()(T x) const BOOST_FIT_RETURNS
    (
        boost::fit::conditional(
            boost::fit::if_(std::is_integral<T>())(boost::fit::pack),
            boost::fit::always(boost::fit::pack())
        )(boost::fit::move(x))
    )
};

struct filter_integers
{
    template<class Seq>
    constexpr auto operator()(Seq s) const BOOST_FIT_RETURNS
    (
        boost::fit::unpack(
            boost::fit::by(integer_predicate(), boost::fit::unpack(make_tuple_f()))
        )(std::move(s))
    )
};


BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(filter_integers()(boost::fit::pack(1, 2, 2.0, 3)) == std::make_tuple(1, 2, 3));
#if BOOST_FIT_HAS_CONSTEXPR_TUPLE
    BOOST_FIT_STATIC_TEST_CHECK(filter_integers()(boost::fit::pack(1, 2, 2.0, 3)) == std::make_tuple(1, 2, 3));
#endif
}


