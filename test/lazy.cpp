#include <boost/fit/lazy.hpp>
#include <memory>
#include "test.hpp"

template<int N>
struct test_placeholder
{};

namespace std {
    template<int N>
    struct is_placeholder<test_placeholder<N>>
    : std::integral_constant<int, N>
    {};
}

BOOST_FIT_TEST_CASE()
{
    int i = 5;

    static_assert(std::is_reference<decltype(boost::fit::detail::ref_transformer()(std::ref(i))(0,0,0))>::value, "Reference wrapper failed");
    static_assert(std::is_reference<decltype(boost::fit::detail::pick_transformer(std::ref(i))(0,0,0))>::value, "Reference wrapper failed");
    static_assert(std::is_reference<decltype(boost::fit::detail::lazy_transform(std::ref(i), boost::fit::pack(0,0,0)))>::value, "Reference wrapper failed");

    BOOST_FIT_TEST_CHECK(&boost::fit::detail::ref_transformer()(std::ref(i))(0,0,0) == &i);
    BOOST_FIT_TEST_CHECK(&boost::fit::detail::pick_transformer(std::ref(i))(0,0,0) == &i);
    BOOST_FIT_TEST_CHECK(&boost::fit::detail::lazy_transform(std::ref(i), boost::fit::pack(0,0,0)) == &i);
}

BOOST_FIT_TEST_CASE()
{
    int i = 5;

    BOOST_FIT_TEST_CHECK(boost::fit::detail::id_transformer()(i)(0,0,0) == i);
    BOOST_FIT_TEST_CHECK(boost::fit::detail::pick_transformer(i)(0,0,0) == i);
    BOOST_FIT_TEST_CHECK(boost::fit::detail::lazy_transform(i, boost::fit::pack(0,0,0)) == i);
}

BOOST_FIT_TEST_CASE()
{
    auto id =[](int i){ return i;};
    auto fi = std::bind(id, 5);

    BOOST_FIT_TEST_CHECK(boost::fit::detail::bind_transformer()(fi)(0,0,0) == id(5));
    BOOST_FIT_TEST_CHECK(boost::fit::detail::pick_transformer(fi)(0,0,0) == id(5));
    BOOST_FIT_TEST_CHECK(boost::fit::detail::lazy_transform(fi, boost::fit::pack(0,0,0)) == id(5));
}

struct f_0 {
constexpr long operator()() const
{
    return 17041L;
}
};

struct f_1 {
constexpr long operator()(long a) const
{
    return a;
}
};

struct f_2 {
constexpr long operator()(long a, long b) const
{
    return a + 10 * b;
}
};

static long global_result;

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
void operator()(long a, long b) const
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

BOOST_FIT_TEST_CASE()
{
    short i(6);

    int const k = 3;

    BOOST_FIT_TEST_CHECK( boost::fit::lazy(Y())( std::ref(i))() == 7 );
    BOOST_FIT_TEST_CHECK( boost::fit::lazy(Y())( std::ref(i))() == 8 );
    BOOST_FIT_TEST_CHECK( boost::fit::lazy(Y())( i,std::placeholders::_1)(k) == 38 );
    BOOST_FIT_TEST_CHECK( boost::fit::lazy(Y())( i,std::placeholders::_1, 9)(k) == 938 );

    global_result = 0;
    boost::fit::lazy(Y())( i,std::placeholders::_1, 9, 4)(k);
    BOOST_FIT_TEST_CHECK( global_result == 4938 );

}

BOOST_FIT_TEST_CASE()
{
    int const x = 1;
    int const y = 2;

    BOOST_FIT_TEST_CHECK( boost::fit::lazy(f_1())( boost::fit::lazy(f_1())(std::placeholders::_1))(x) == 1L );
    BOOST_FIT_TEST_CHECK( boost::fit::lazy(f_1())( boost::fit::lazy(f_2())(std::placeholders::_1, std::placeholders::_2))(x, y) == 21L );
    BOOST_FIT_TEST_CHECK( boost::fit::lazy(f_2())( boost::fit::lazy(f_1())(std::placeholders::_1), boost::fit::lazy(f_1())(std::placeholders::_1))(x) == 11L );
    BOOST_FIT_TEST_CHECK( boost::fit::lazy(f_2())( boost::fit::lazy(f_1())(std::placeholders::_1), boost::fit::lazy(f_1())( std::placeholders::_2))(x, y) == 21L );
    BOOST_FIT_TEST_CHECK( boost::fit::lazy(f_1())( boost::fit::lazy(f_0())())() == 17041L );

    BOOST_FIT_STATIC_TEST_CHECK( boost::fit::lazy(f_1())( boost::fit::lazy(f_1())(test_placeholder<1>()))(x) == 1L );
    BOOST_FIT_STATIC_TEST_CHECK( boost::fit::lazy(f_1())( boost::fit::lazy(f_2())(test_placeholder<1>(), test_placeholder<2>()))(x, y) == 21L );
    BOOST_FIT_STATIC_TEST_CHECK( boost::fit::lazy(f_2())( boost::fit::lazy(f_1())(test_placeholder<1>()), boost::fit::lazy(f_1())(test_placeholder<1>()))(x) == 11L );
    BOOST_FIT_STATIC_TEST_CHECK( boost::fit::lazy(f_2())( boost::fit::lazy(f_1())(test_placeholder<1>()), boost::fit::lazy(f_1())( test_placeholder<2>()))(x, y) == 21L );
    BOOST_FIT_STATIC_TEST_CHECK( boost::fit::lazy(f_1())( boost::fit::lazy(f_0())())() == 17041L );

    BOOST_FIT_TEST_CHECK( (boost::fit::lazy(fv_1())( boost::fit::lazy(f_1())(std::placeholders::_1))(x), (global_result == 1L)) );
    BOOST_FIT_TEST_CHECK( (boost::fit::lazy(fv_1())( boost::fit::lazy(f_2())(std::placeholders::_1, std::placeholders::_2))(x, y), (global_result == 21L)) );
    BOOST_FIT_TEST_CHECK( (boost::fit::lazy(fv_2())( boost::fit::lazy(f_1())(std::placeholders::_1), boost::fit::lazy(f_1())(std::placeholders::_1))(x), (global_result == 11L)) );
    BOOST_FIT_TEST_CHECK( (boost::fit::lazy(fv_2())( boost::fit::lazy(f_1())(std::placeholders::_1), boost::fit::lazy(f_1())( std::placeholders::_2))(x, y), (global_result == 21L)) );
    BOOST_FIT_TEST_CHECK( (boost::fit::lazy(fv_1())( boost::fit::lazy(f_0())())(), (global_result == 17041L)) );
}

struct id
{
    int operator()(int i) const
    {
        return i;
    }
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::lazy(id())(3)() == 3);
}

struct deref
{
    int operator()(const std::unique_ptr<int>& i) const
    {
        return *i;
    }
};

BOOST_FIT_TEST_CASE()
{
    BOOST_FIT_TEST_CHECK(boost::fit::lazy(deref())(std::unique_ptr<int>(new int(3)))() == 3);
}
