#include <fit/unpack.hpp>
#include <fit/static.hpp>
#include <fit/lambda.hpp>
#include "test.hpp"

#include <memory>

static constexpr fit::static_<fit::unpack_adaptor<unary_class> > unary_unpack = {};
static constexpr fit::static_<fit::unpack_adaptor<binary_class> > binary_unpack = {};

FIT_STATIC_AUTO unary_unpack_constexpr = fit::unpack_adaptor<unary_class>();
FIT_STATIC_AUTO binary_unpack_constexpr = fit::unpack_adaptor<binary_class>();

FIT_STATIC_AUTO unary_unpack_reveal = fit::reveal_adaptor<fit::unpack_adaptor<unary_class>>();
FIT_STATIC_AUTO binary_unpack_reveal = fit::reveal_adaptor<fit::unpack_adaptor<binary_class>>();

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::unpack(unary_class())(std::make_tuple(3)));
    FIT_TEST_CHECK(3 == unary_unpack(std::make_tuple(3)));
    FIT_TEST_CHECK(3 == unary_unpack_reveal(std::make_tuple(3)));
    int ifu = 3;
    FIT_TEST_CHECK(3 == unary_unpack(std::tuple<int&>(ifu)));

    FIT_STATIC_TEST_CHECK(3 == fit::unpack(unary_class())(std::make_tuple(3)));
    FIT_STATIC_TEST_CHECK(3 == unary_unpack_constexpr(std::make_tuple(3)));
    FIT_STATIC_TEST_CHECK(3 == unary_unpack_reveal(std::make_tuple(3)));
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::unpack(unary_class())(fit::pack(3)));
    FIT_TEST_CHECK(3 == unary_unpack(fit::pack(3)));
    FIT_TEST_CHECK(3 == unary_unpack_reveal(fit::pack(3)));
    int ifu = 3;
    FIT_TEST_CHECK(3 == unary_unpack(fit::pack_forward(ifu)));

    FIT_STATIC_TEST_CHECK(3 == fit::unpack(unary_class())(fit::pack(3)));
    FIT_STATIC_TEST_CHECK(3 == unary_unpack_constexpr(fit::pack(3)));
    FIT_STATIC_TEST_CHECK(3 == unary_unpack_reveal(fit::pack(3)));
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1, 2)));
    FIT_TEST_CHECK(3 == binary_unpack(std::make_tuple(1, 2)));
    FIT_TEST_CHECK(3 == binary_unpack_reveal(std::make_tuple(1, 2)));

    FIT_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1), std::make_tuple(2)));
    FIT_TEST_CHECK(3 == binary_unpack(std::make_tuple(1), std::make_tuple(2)));
    FIT_TEST_CHECK(3 == binary_unpack_reveal(std::make_tuple(1), std::make_tuple(2)));

    FIT_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));
    FIT_TEST_CHECK(3 == binary_unpack(std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));
    FIT_TEST_CHECK(3 == binary_unpack_reveal(std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));

    FIT_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(), std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));
    FIT_TEST_CHECK(3 == binary_unpack(std::make_tuple(), std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));
    FIT_TEST_CHECK(3 == binary_unpack_reveal(std::make_tuple(), std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));

    FIT_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1), std::make_tuple(), std::make_tuple(2), std::make_tuple()));
    FIT_TEST_CHECK(3 == binary_unpack(std::make_tuple(1), std::make_tuple(), std::make_tuple(2), std::make_tuple()));
    FIT_TEST_CHECK(3 == binary_unpack_reveal(std::make_tuple(1), std::make_tuple(), std::make_tuple(2), std::make_tuple()));

    FIT_STATIC_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1, 2)));
    FIT_STATIC_TEST_CHECK(3 == binary_unpack_constexpr(std::make_tuple(1, 2)));
    FIT_STATIC_TEST_CHECK(3 == binary_unpack_reveal(std::make_tuple(1, 2)));

    FIT_STATIC_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1), std::make_tuple(2)));
    FIT_STATIC_TEST_CHECK(3 == binary_unpack_constexpr(std::make_tuple(1), std::make_tuple(2)));
    FIT_STATIC_TEST_CHECK(3 == binary_unpack_reveal(std::make_tuple(1), std::make_tuple(2)));

    FIT_STATIC_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));
    FIT_STATIC_TEST_CHECK(3 == binary_unpack_constexpr(std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));
    FIT_STATIC_TEST_CHECK(3 == binary_unpack_reveal(std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));

    FIT_STATIC_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(), std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));
    FIT_STATIC_TEST_CHECK(3 == binary_unpack_constexpr(std::make_tuple(), std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));
    FIT_STATIC_TEST_CHECK(3 == binary_unpack_reveal(std::make_tuple(), std::make_tuple(1), std::make_tuple(), std::make_tuple(2)));

    FIT_STATIC_TEST_CHECK(3 == fit::unpack(binary_class())(std::make_tuple(1), std::make_tuple(), std::make_tuple(2), std::make_tuple()));
    FIT_STATIC_TEST_CHECK(3 == binary_unpack_constexpr(std::make_tuple(1), std::make_tuple(), std::make_tuple(2), std::make_tuple()));
    FIT_STATIC_TEST_CHECK(3 == binary_unpack_reveal(std::make_tuple(1), std::make_tuple(), std::make_tuple(2), std::make_tuple()));
}

FIT_TEST_CASE()
{
    auto p1 = fit::pack_basic(1, 2);
    static_assert(fit::is_unpackable<decltype(p1)>::value, "Not unpackable");
    static_assert(fit::is_unpackable<decltype((p1))>::value, "Not unpackable");

    auto p2 = fit::pack_forward(1, 2);
    static_assert(fit::is_unpackable<decltype(p2)>::value, "Not unpackable");
    static_assert(fit::is_unpackable<decltype((p2))>::value, "Not unpackable");

    auto p3 = fit::pack(1, 2);
    static_assert(fit::is_unpackable<decltype(p3)>::value, "Not unpackable");
    static_assert(fit::is_unpackable<decltype((p3))>::value, "Not unpackable");

    static_assert(fit::is_unpackable<std::tuple<int>>::value, "Not unpackable");
    
    static_assert(!fit::is_unpackable<int>::value, "Unpackable");
    static_assert(!fit::is_unpackable<void>::value, "Unpackable");
}

FIT_TEST_CASE()
{
    typedef std::tuple<int, int> tuple_type;
    static_assert(fit::is_unpackable<tuple_type>::value, "Not unpackable");
    static_assert(fit::is_unpackable<tuple_type&>::value, "Not unpackable");
    static_assert(fit::is_unpackable<const tuple_type&>::value, "Not unpackable");
    static_assert(fit::is_unpackable<tuple_type&&>::value, "Not unpackable");
    
}

FIT_STATIC_AUTO lambda_unary_unpack = fit::unpack(FIT_STATIC_LAMBDA(int x)
{
    return x;
});

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == lambda_unary_unpack(std::make_tuple(3)));
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == lambda_unary_unpack(fit::pack(3)));
}

struct unary_move
{
    std::unique_ptr<int> i;
    unary_move()
    : i(new int(2))
    {}

    template<class T>
    T operator()(T x) const
    {
        return x + *i;
    }
};

static constexpr fit::static_<fit::unpack_adaptor<unary_move> > unary_move_unpack = {};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::unpack(unary_move())(std::make_tuple(1)));
    FIT_TEST_CHECK(3 == unary_move_unpack(std::make_tuple(1)));
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::unpack(unary_move())(fit::pack(1)));
    FIT_TEST_CHECK(3 == unary_move_unpack(fit::pack(1)));
}

struct indirect_sum_f
{
    template<class T, class U>
    auto operator()(T x, U y) const
    FIT_RETURNS(*x + *y);
};

#define MAKE_UNIQUE_PTR(x) std::unique_ptr<int>(new int(x)) 

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(3 == fit::unpack(indirect_sum_f())(fit::pack_basic(MAKE_UNIQUE_PTR(1), MAKE_UNIQUE_PTR(2))));
    FIT_TEST_CHECK(3 == fit::unpack(indirect_sum_f())(fit::pack_forward(MAKE_UNIQUE_PTR(1), MAKE_UNIQUE_PTR(2))));
    FIT_TEST_CHECK(3 == fit::unpack(indirect_sum_f())(fit::pack(MAKE_UNIQUE_PTR(1), MAKE_UNIQUE_PTR(2))));
    FIT_TEST_CHECK(3 == fit::unpack(indirect_sum_f())(std::make_tuple(MAKE_UNIQUE_PTR(1), MAKE_UNIQUE_PTR(2))));
}

template<class...>
struct deduce_types
{};

struct deducer
{
    template<class... Ts>
    deduce_types<Ts...> operator()(Ts&&...) const;
};

static constexpr fit::unpack_adaptor<deducer> deduce = {};

FIT_TEST_CASE()
{
    STATIC_ASSERT_SAME(deduce_types<int, int>, decltype(deduce(std::make_tuple(1, 2))));
    STATIC_ASSERT_SAME(deduce_types<int, int>, decltype(deduce(std::make_tuple(1), std::make_tuple(2))));
    STATIC_ASSERT_SAME(deduce_types<int, int, int>, decltype(deduce(std::make_tuple(1), std::make_tuple(2), std::make_tuple(3))));
    STATIC_ASSERT_SAME(std::tuple<int&&, int&&>, decltype(std::forward_as_tuple(1, 2)));
    // Disable this test, it seems that rvalue references get swalllowed by type deduction
    // STATIC_ASSERT_SAME(deduce_types<int&&, int&&>, decltype(deduce(std::forward_as_tuple(1, 2))));


    STATIC_ASSERT_SAME(deduce_types<int, int>, decltype(deduce(fit::pack_basic(1, 2))));
    STATIC_ASSERT_SAME(deduce_types<int, int>, decltype(deduce(fit::pack_basic(1), fit::pack_basic(2))));
    STATIC_ASSERT_SAME(deduce_types<int, int, int>, decltype(deduce(fit::pack_basic(1), fit::pack_basic(2), fit::pack_basic(3))));
    // STATIC_ASSERT_SAME(deduce_types<int&&, int&&>, decltype(deduce(fit::pack_forward(1, 2))));
}

struct not_unpackable
{};

FIT_TEST_CASE()
{
    auto f = fit::unpack(fit::always(1));

    static_assert(!fit::is_callable<decltype(f), not_unpackable>::value, "SFINAE for unpack failed");
}
