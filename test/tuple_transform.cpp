#include <fit/by.hpp>
#include <fit/construct.hpp>
#include <fit/unpack.hpp>
#include <fit/function.hpp>
#include <fit/placeholders.hpp>
#include <fit/compose.hpp>
#include "test.hpp"

struct tuple_transform_f
{
    template<class Sequence, class F>
    constexpr auto operator()(Sequence&& s, F f) const FIT_RETURNS
    (
        fit::unpack(fit::by(f, fit::construct<std::tuple>()))(fit::forward<Sequence>(s))
    );
};

struct pack_transform_f
{
    template<class Sequence, class F>
    constexpr auto operator()(Sequence&& s, F f) const FIT_RETURNS
    (
        fit::unpack(fit::by(f, fit::pack()))(fit::forward<Sequence>(s))
    );
};

FIT_STATIC_FUNCTION(tuple_transform) = tuple_transform_f{};
// FIT_STATIC_FUNCTION(pack_transform) = pack_transform_f{};

#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8
#define TUPLE_TRANSFORM_STATIC_CHECK(...)
#else
#define TUPLE_TRANSFORM_STATIC_CHECK FIT_STATIC_TEST_CHECK

#endif

FIT_TEST_CASE()
{
    auto t = std::make_tuple(1, 2);
    auto r = tuple_transform(t, [](int i) { return i*i; });
    FIT_TEST_CHECK(r == std::make_tuple(1, 4));
}

FIT_TEST_CASE()
{
    TUPLE_TRANSFORM_STATIC_CHECK(tuple_transform(std::make_tuple(1, 2), fit::_1 * fit::_1) == std::make_tuple(1, 4));
}

#define TUPLE_TRANSFORM_CHECK_ID(x) \
FIT_TEST_CHECK(tuple_transform(x, fit::identity) == x); \
TUPLE_TRANSFORM_STATIC_CHECK(tuple_transform(x, fit::identity) == x);

FIT_TEST_CASE()
{
    TUPLE_TRANSFORM_CHECK_ID(std::make_tuple(1, 2));
    TUPLE_TRANSFORM_CHECK_ID(std::make_tuple(1));
    TUPLE_TRANSFORM_CHECK_ID(std::make_tuple());
}

FIT_TEST_CASE()
{
    auto x = tuple_transform(std::make_tuple(std::unique_ptr<int>(new int(3))), fit::identity);
    auto y = std::make_tuple(std::unique_ptr<int>(new int(3)));
    FIT_TEST_CHECK(x != y);
    FIT_TEST_CHECK(tuple_transform(x, *fit::_1) == tuple_transform(y, *fit::_1));
}

#define TUPLE_TRANSFORM_CHECK_COMPOSE(x, f, g) \
FIT_TEST_CHECK(tuple_transform(x, fit::compose(f, g)) == tuple_transform(tuple_transform(x, g), f)); \
TUPLE_TRANSFORM_STATIC_CHECK(tuple_transform(x, fit::compose(f, g)) == tuple_transform(tuple_transform(x, g), f));

FIT_TEST_CASE()
{
    TUPLE_TRANSFORM_CHECK_COMPOSE(std::make_tuple(1, 2, 3, 4), fit::_1 * fit::_1, fit::_1 + fit::_1);
    TUPLE_TRANSFORM_CHECK_COMPOSE(std::make_tuple(1, 2, 3), fit::_1 * fit::_1, fit::_1 + fit::_1);
    TUPLE_TRANSFORM_CHECK_COMPOSE(std::make_tuple(1), fit::_1 * fit::_1, fit::_1 + fit::_1);
    TUPLE_TRANSFORM_CHECK_COMPOSE(std::make_tuple(), fit::_1 * fit::_1, fit::_1 + fit::_1);
}
