#include <fit/lazy.h>
#include "test.h"

// TODO: Test constexpr

FIT_TEST_CASE()
{
    int i = 5;

    static_assert(std::is_reference<decltype(fit::detail::ref_transformer()(std::ref(i))(0,0,0))>::value, "Reference wrapper failed");
    static_assert(std::is_reference<decltype(fit::detail::pick_transformer(std::ref(i))(0,0,0))>::value, "Reference wrapper failed");
    static_assert(std::is_reference<decltype(fit::detail::lazy_transform(std::ref(i), 0,0,0))>::value, "Reference wrapper failed");

    FIT_TEST_CHECK(&fit::detail::ref_transformer()(std::ref(i))(0,0,0) == &i);
    FIT_TEST_CHECK(&fit::detail::pick_transformer(std::ref(i))(0,0,0) == &i);
    FIT_TEST_CHECK(&fit::detail::lazy_transform(std::ref(i), 0,0,0) == &i);
}

FIT_TEST_CASE()
{
    int i = 5;

    FIT_TEST_CHECK(fit::detail::id_transformer()(i)(0,0,0) == i);
    FIT_TEST_CHECK(fit::detail::pick_transformer(i)(0,0,0) == i);
    FIT_TEST_CHECK(fit::detail::lazy_transform(i, 0,0,0) == i);
}

FIT_TEST_CASE()
{
    auto id =[](int i){ return i;};
    auto fi = std::bind(id, 5);

    FIT_TEST_CHECK(fit::detail::bind_transformer()(fi)(0,0,0) == id(5));
    FIT_TEST_CHECK(fit::detail::pick_transformer(fi)(0,0,0) == id(5));
    FIT_TEST_CHECK(fit::detail::lazy_transform(fi, 0,0,0) == id(5));
}

struct f_0 {
long operator()() const
{
    return 17041L;
}
};

struct f_1 {
long operator()(long a) const
{
    return a;
}
};

struct f_2 {
long operator()(long a, long b) const
{
    return a + 10 * b;
}
};

long global_result;

struct fv_0 {
void operator()() const
{
    global_result = 17041L;
}
};

struct fv_1 {
void operator()(long a) const
{
    global_result = a;
}
};

struct fv_2 {
void operator()(long a, long b)
{
    global_result = a + 10 * b;
}
};

struct Y
{
    short operator()(short & r) const { return ++r; }
    int operator()(int a, int b) const { return a + 10 * b; }
    long operator() (long a, long b, long c) const { return a + 10 * b + 100 * c; }
    void operator() (long a, long b, long c, long d) const { global_result = a + 10 * b + 100 * c + 1000 * d; }
};

FIT_TEST_CASE()
{
    short i(6);

    int const k = 3;

    FIT_TEST_CHECK( fit::lazy(Y())( std::ref(i))() == 7 );
    FIT_TEST_CHECK( fit::lazy(Y())( std::ref(i))() == 8 );
    FIT_TEST_CHECK( fit::lazy(Y())( i,std::placeholders::_1)(k) == 38 );
    FIT_TEST_CHECK( fit::lazy(Y())( i,std::placeholders::_1, 9)(k) == 938 );

    global_result = 0;
    fit::lazy(Y())( i,std::placeholders::_1, 9, 4)(k);
    FIT_TEST_CHECK( global_result == 4938 );

}

FIT_TEST_CASE()
{
    int const x = 1;
    int const y = 2;

    FIT_TEST_CHECK( fit::lazy(f_1())( fit::lazy(f_1())(std::placeholders::_1))(x) == 1L );
    FIT_TEST_CHECK( fit::lazy(f_1())( fit::lazy(f_2())(std::placeholders::_1, std::placeholders::_2))(x, y) == 21L );
    FIT_TEST_CHECK( fit::lazy(f_2())( fit::lazy(f_1())(std::placeholders::_1), fit::lazy(f_1())(std::placeholders::_1))(x) == 11L );
    FIT_TEST_CHECK( fit::lazy(f_2())( fit::lazy(f_1())(std::placeholders::_1), fit::lazy(f_1())( std::placeholders::_2))(x, y) == 21L );
    FIT_TEST_CHECK( fit::lazy(f_1())( fit::lazy(f_0())())() == 17041L );

    FIT_TEST_CHECK( (fit::lazy(fv_1())( fit::lazy(f_1())(std::placeholders::_1))(x), (global_result == 1L)) );
    FIT_TEST_CHECK( (fit::lazy(fv_1())( fit::lazy(f_2())(std::placeholders::_1, std::placeholders::_2))(x, y), (global_result == 21L)) );
    FIT_TEST_CHECK( (fit::lazy(fv_2())( fit::lazy(f_1())(std::placeholders::_1), fit::lazy(f_1())(std::placeholders::_1))(x), (global_result == 11L)) );
    FIT_TEST_CHECK( (fit::lazy(fv_2())( fit::lazy(f_1())(std::placeholders::_1), fit::lazy(f_1())( std::placeholders::_2))(x, y), (global_result == 21L)) );
    FIT_TEST_CHECK( (fit::lazy(fv_1())( fit::lazy(f_0())())(), (global_result == 17041L)) );
}
