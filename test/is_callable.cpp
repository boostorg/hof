#include <boost/fit/is_callable.hpp>
#include <ciso646>
#include "test.hpp"

template<int N>
struct callable_rank : callable_rank<N-1>
{};

template<>
struct callable_rank<0>
{};

BOOST_FIT_STATIC_TEST_CASE()
{
    struct is_callable_class
    {
        void operator()(int) const
        {
        }
    };
    struct callable_test_param {};

    void is_callable_function(int)
    {
    }

    struct is_callable_rank_class
    {
        void operator()(int, callable_rank<3>) const
        {
        }

        void operator()(int, callable_rank<4>) const
        {
        }
    };

    static_assert(boost::fit::is_callable<is_callable_class, int>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_class, long>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_class, double>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_class, const int&>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_class, const long&>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_class, const double&>::value, "Not callable");
    static_assert(not boost::fit::is_callable<is_callable_class, callable_test_param>::value, "callable failed");
    static_assert(not boost::fit::is_callable<is_callable_class>::value, "callable failed");
    static_assert(not boost::fit::is_callable<is_callable_class, int, int>::value, "callable failed");

    typedef void (*is_callable_function_pointer)(int);
    static_assert(boost::fit::is_callable<is_callable_function_pointer, int>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_function_pointer, long>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_function_pointer, double>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_function_pointer, const int&>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_function_pointer, const long&>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_function_pointer, const double&>::value, "Not callable");
    static_assert(not boost::fit::is_callable<is_callable_function_pointer, callable_test_param>::value, "callable failed");
    static_assert(not boost::fit::is_callable<is_callable_function_pointer>::value, "callable failed");
    static_assert(not boost::fit::is_callable<is_callable_function_pointer, int, int>::value, "callable failed");

    static_assert(boost::fit::is_callable<is_callable_rank_class, int, callable_rank<3>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, long, callable_rank<3>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, double, callable_rank<3>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, const int&, callable_rank<3>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, const long&, callable_rank<3>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, const double&, callable_rank<3>>::value, "Not callable");

    static_assert(boost::fit::is_callable<is_callable_rank_class, int, callable_rank<4>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, long, callable_rank<4>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, double, callable_rank<4>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, const int&, callable_rank<4>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, const long&, callable_rank<4>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, const double&, callable_rank<4>>::value, "Not callable");

    static_assert(boost::fit::is_callable<is_callable_rank_class, int, callable_rank<5>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, long, callable_rank<5>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, double, callable_rank<5>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, const int&, callable_rank<5>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, const long&, callable_rank<5>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, const double&, callable_rank<5>>::value, "Not callable");

    static_assert(boost::fit::is_callable<is_callable_rank_class, int, callable_rank<6>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, long, callable_rank<6>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, double, callable_rank<6>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, const int&, callable_rank<6>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, const long&, callable_rank<6>>::value, "Not callable");
    static_assert(boost::fit::is_callable<is_callable_rank_class, const double&, callable_rank<6>>::value, "Not callable");

    static_assert(not boost::fit::is_callable<is_callable_rank_class, int, callable_rank<1>>::value, "callable failed");
    static_assert(not boost::fit::is_callable<is_callable_rank_class, long, callable_rank<1>>::value, "callable failed");
    static_assert(not boost::fit::is_callable<is_callable_rank_class, double, callable_rank<1>>::value, "callable failed");
    static_assert(not boost::fit::is_callable<is_callable_rank_class, const int&, callable_rank<1>>::value, "callable failed");
    static_assert(not boost::fit::is_callable<is_callable_rank_class, const long&, callable_rank<1>>::value, "callable failed");
    static_assert(not boost::fit::is_callable<is_callable_rank_class, const double&, callable_rank<1>>::value, "callable failed");

    static_assert(not boost::fit::is_callable<is_callable_rank_class, callable_test_param, callable_test_param>::value, "callable failed");
    static_assert(not boost::fit::is_callable<is_callable_rank_class, callable_rank<3>, callable_test_param>::value, "callable failed");
    static_assert(not boost::fit::is_callable<is_callable_rank_class, callable_rank<4>, callable_test_param>::value, "callable failed");
    static_assert(not boost::fit::is_callable<is_callable_rank_class, callable_test_param, callable_rank<3>>::value, "callable failed");
    static_assert(not boost::fit::is_callable<is_callable_rank_class, callable_test_param, callable_rank<4>>::value, "callable failed");
    static_assert(not boost::fit::is_callable<is_callable_rank_class>::value, "callable failed");
    static_assert(not boost::fit::is_callable<is_callable_rank_class, int, int>::value, "callable failed");
};
