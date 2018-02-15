#include <boost/hof/if.hpp>
#include "test.hpp"

#include <boost/hof/proj.hpp>
#include <boost/hof/lift.hpp>
#include <boost/hof/construct.hpp>
#include <boost/hof/conditional.hpp>
#include <boost/hof/unpack.hpp>

#include <tuple>

#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8)
#define BOOST_HOF_HAS_CONSTEXPR_TUPLE 0
#else
#define BOOST_HOF_HAS_CONSTEXPR_TUPLE 1
#endif


BOOST_HOF_LIFT_CLASS(make_tuple_f, std::make_tuple);

struct integer_predicate
{
    constexpr integer_predicate()
    {}
    template<class T>
    constexpr auto operator()(T x) const BOOST_HOF_RETURNS
    (
        boost::hof::conditional(
            boost::hof::if_(std::is_integral<T>())(boost::hof::pack_basic),
            boost::hof::always(boost::hof::pack_basic())
        )(boost::hof::move(x))
    )
};

struct filter_integers
{
    template<class Seq>
    constexpr auto operator()(Seq s) const BOOST_HOF_RETURNS
    (
        boost::hof::unpack(
            boost::hof::proj(integer_predicate(), boost::hof::unpack(make_tuple_f()))
        )(std::move(s))
    )
};


BOOST_HOF_TEST_CASE()
{
    BOOST_HOF_TEST_CHECK(filter_integers()(boost::hof::pack_basic(1, 2, 2.0, 3)) == std::make_tuple(1, 2, 3));
#if BOOST_HOF_HAS_CONSTEXPR_TUPLE
    BOOST_HOF_STATIC_TEST_CHECK(filter_integers()(boost::hof::pack_basic(1, 2, 2.0, 3)) == std::make_tuple(1, 2, 3));
#endif
}


