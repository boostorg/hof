#include <fit/unpack.hpp>
#include <fit/by.hpp>
#include <fit/function.hpp>
#include <fit/reveal.hpp>
#include "test.hpp"

struct tuple_for_each_f
{
    template<class Sequence, class F>
    constexpr auto operator()(Sequence&& s, F && f) const FIT_RETURNS
    (
        fit::unpack(fit::by(fit::forward<F>(f)))(fit::forward<Sequence>(s)), fit::forward<F>(f)
    );
};

FIT_STATIC_FUNCTION(tuple_for_each) = tuple_for_each_f{};

FIT_TEST_CASE()
{
    std::tuple<int, short, char> tp{ 1, 2, 3 };

    {
        int s = 0;

        tuple_for_each( tp, [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 123 );
    }

    {
        int s = 0;

        tuple_for_each( std::move(tp), [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 123 );
    }
}
FIT_TEST_CASE()
{
    std::tuple<int, short, char> const tp{ 1, 2, 3 };

    {
        int s = 0;

        tuple_for_each( tp, [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 123 );
    }

    {
        int s = 0;

        tuple_for_each( std::move(tp), [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 123 );
    }
}

// #if defined( __clang_major__ ) && __clang_major__ == 3 && __clang_minor__ < 8
// #else
FIT_TEST_CASE()
{
    std::tuple<std::unique_ptr<int>, std::unique_ptr<int>, std::unique_ptr<int>> tp{ std::unique_ptr<int>(new int(1)), std::unique_ptr<int>(new int(2)), std::unique_ptr<int>(new int(3)) };

    int s = 0;

    tuple_for_each( std::move(tp), [&]( std::unique_ptr<int> p ){ s = s * 10 + *p; } );

    FIT_TEST_CHECK( s == 123 );
}

FIT_TEST_CASE()
{
    auto tp = fit::pack(1, 2, 3);

    {
        int s = 0;

        tuple_for_each( tp, [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 123 );
    }

    {
        int s = 0;

        tuple_for_each( std::move(tp), [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 123 );
    }
}
FIT_TEST_CASE()
{
    const auto tp = fit::pack(1, 2, 3);

    {
        int s = 0;

        tuple_for_each( tp, [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 123 );
    }

    {
        int s = 0;

        tuple_for_each( std::move(tp), [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 123 );
    }
}
// #endif
FIT_TEST_CASE()
{
    std::pair<int, short> tp{ 1, 2 };

    {
        int s = 0;

        tuple_for_each( tp, [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 12 );
    }

    {
        int s = 0;

        tuple_for_each( std::move(tp), [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 12 );
    }
}  
FIT_TEST_CASE()
{
    std::pair<int, short> const tp{ 1, 2 };

    {
        int s = 0;

        tuple_for_each( tp, [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 12 );
    }

    {
        int s = 0;

        tuple_for_each( std::move(tp), [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 12 );
    }
}
FIT_TEST_CASE()
{
    std::array<int, 3> tp{{ 1, 2, 3 }};

    {
        int s = 0;

        tuple_for_each( tp, [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 123 );
    }

    {
        int s = 0;

        tuple_for_each( std::move(tp), [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 123 );
    }
}
FIT_TEST_CASE()
{
    std::array<int, 3> const tp{{ 1, 2, 3 }};

    {
        int s = 0;

        tuple_for_each( tp, [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 123 );
    }

    {
        int s = 0;

        tuple_for_each( std::move(tp), [&]( int x ){ s = s * 10 + x; } );

        FIT_TEST_CHECK( s == 123 );
    }
}
FIT_TEST_CASE()
{
    std::tuple<> tp;

    FIT_TEST_CHECK( tuple_for_each( tp, 11 ) == 11 );
    FIT_TEST_CHECK( tuple_for_each( std::move( tp ), 12 ) == 12 );
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK( tuple_for_each( fit::pack(), 11 ) == 11 );
    FIT_STATIC_TEST_CHECK( tuple_for_each( fit::pack(), 11 ) == 11 );
}
FIT_TEST_CASE()
{
    std::array<int, 0> tp;

    FIT_TEST_CHECK( tuple_for_each( tp, 11 ) == 11 );
    FIT_TEST_CHECK( tuple_for_each( std::move( tp ), 12 ) == 12 );
}

struct assert_is_integral
{
    template<class T> constexpr bool operator()( T ) const
    {
        FIT_STATIC_TEST_CHECK( std::is_integral<T>::value );
        return true;
    }
};

FIT_TEST_CASE()
{
#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7)
    auto r = tuple_for_each( std::tuple<int, short, char>{1, 2, 3}, assert_is_integral() );
#else
    constexpr auto r = tuple_for_each( std::tuple<int, short, char>{1, 2, 3}, assert_is_integral() );
#endif
    (void)r;
}

FIT_TEST_CASE()
{
#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7)
    auto r = tuple_for_each( fit::pack(1, 2, 3), assert_is_integral() );
#else
    constexpr auto r = tuple_for_each( fit::pack(1, 2, 3), assert_is_integral() );
#endif
    (void)r;
}
