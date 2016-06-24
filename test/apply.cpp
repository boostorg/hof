#include <fit/apply.hpp>
#include "test.hpp"

FIT_TEST_CASE()
{    
    FIT_STATIC_TEST_CHECK(fit::apply(binary_class(), 1, 2) == 3);
    FIT_TEST_CHECK(fit::apply(binary_class(), 1, 2) == 3);
}

struct member_sum_f
{
    int i;
    constexpr member_sum_f(int x) : i(x)
    {}

    constexpr int add(int x) const
    {
        return i+x;
    }
};

struct member_sum_f_derived
: member_sum_f
{
    constexpr member_sum_f_derived(int x) : member_sum_f(x)
    {}
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::apply(&member_sum_f::add, member_sum_f(1), 2) == 3);
    FIT_TEST_CHECK(fit::apply(&member_sum_f::add, member_sum_f_derived(1), 2) == 3);

#ifdef __clang__
    FIT_STATIC_TEST_CHECK(fit::apply(&member_sum_f::add, member_sum_f(1), 2) == 3);
    FIT_STATIC_TEST_CHECK(fit::apply(&member_sum_f::add, member_sum_f_derived(1), 2) == 3);
#endif

    static_assert(std::is_base_of<member_sum_f, member_sum_f>::value, "Base of failed");
    std::unique_ptr<member_sum_f> msp(new member_sum_f(1));
    FIT_TEST_CHECK(fit::apply(&member_sum_f::add, msp, 2) == 3);

    std::unique_ptr<member_sum_f_derived> mspd(new member_sum_f_derived(1));
    FIT_TEST_CHECK(fit::apply(&member_sum_f::add, mspd, 2) == 3);
}

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::apply(&member_sum_f::i, member_sum_f(3)) == 3);
    FIT_TEST_CHECK(fit::apply(&member_sum_f::i, member_sum_f_derived(3)) == 3);

#ifdef __clang__
    FIT_STATIC_TEST_CHECK(fit::apply(&member_sum_f::i, member_sum_f(3)) == 3);
    FIT_STATIC_TEST_CHECK(fit::apply(&member_sum_f::i, member_sum_f_derived(3)) == 3);
#endif

    std::unique_ptr<member_sum_f> msp(new member_sum_f(3));
    FIT_TEST_CHECK(fit::apply(&member_sum_f::i, msp) == 3);

    std::unique_ptr<member_sum_f_derived> mspd(new member_sum_f_derived(3));
    FIT_TEST_CHECK(fit::apply(&member_sum_f::i, mspd) == 3);
}


struct mem_hash
{
    mutable unsigned int hash;

    mem_hash(): hash(0) {}

    int f0() { f1(17); return 0; }
    int g0() const { g1(17); return 0; }

    int f1(int a1) { hash = (hash * 17041 + a1) % 32768; return 0; }
    int g1(int a1) const { hash = (hash * 17041 + a1 * 2) % 32768; return 0; }

    int f2(int a1, int a2) { f1(a1); f1(a2); return 0; }
    int g2(int a1, int a2) const { g1(a1); g1(a2); return 0; }

    int f3(int a1, int a2, int a3) { f2(a1, a2); f1(a3); return 0; }
    int g3(int a1, int a2, int a3) const { g2(a1, a2); g1(a3); return 0; }

    int f4(int a1, int a2, int a3, int a4) { f3(a1, a2, a3); f1(a4); return 0; }
    int g4(int a1, int a2, int a3, int a4) const { g3(a1, a2, a3); g1(a4); return 0; }

    int f5(int a1, int a2, int a3, int a4, int a5) { f4(a1, a2, a3, a4); f1(a5); return 0; }
    int g5(int a1, int a2, int a3, int a4, int a5) const { g4(a1, a2, a3, a4); g1(a5); return 0; }

    int f6(int a1, int a2, int a3, int a4, int a5, int a6) { f5(a1, a2, a3, a4, a5); f1(a6); return 0; }
    int g6(int a1, int a2, int a3, int a4, int a5, int a6) const { g5(a1, a2, a3, a4, a5); g1(a6); return 0; }

    int f7(int a1, int a2, int a3, int a4, int a5, int a6, int a7) { f6(a1, a2, a3, a4, a5, a6); f1(a7); return 0; }
    int g7(int a1, int a2, int a3, int a4, int a5, int a6, int a7) const { g6(a1, a2, a3, a4, a5, a6); g1(a7); return 0; }

    int f8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) { f7(a1, a2, a3, a4, a5, a6, a7); f1(a8); return 0; }
    int g8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) const { g7(a1, a2, a3, a4, a5, a6, a7); g1(a8); return 0; }
};

FIT_TEST_CASE()
{

    mem_hash x;

    mem_hash const & rcx = x;
    mem_hash const * pcx = &x;

    std::shared_ptr<mem_hash> sp(new mem_hash);

    fit::apply(&mem_hash::f0, x);
    fit::apply(&mem_hash::f0, &x);
    fit::apply(&mem_hash::f0, sp);

    fit::apply(&mem_hash::g0, x);
    fit::apply(&mem_hash::g0, rcx);
    fit::apply(&mem_hash::g0, &x);
    fit::apply(&mem_hash::g0, pcx);
    fit::apply(&mem_hash::g0, sp);

    fit::apply(&mem_hash::f1, x, 1);
    fit::apply(&mem_hash::f1, &x, 1);
    fit::apply(&mem_hash::f1, sp, 1);

    fit::apply(&mem_hash::g1, x, 1);
    fit::apply(&mem_hash::g1, rcx, 1);
    fit::apply(&mem_hash::g1, &x, 1);
    fit::apply(&mem_hash::g1, pcx, 1);
    fit::apply(&mem_hash::g1, sp, 1);

    fit::apply(&mem_hash::f2, x, 1, 2);
    fit::apply(&mem_hash::f2, &x, 1, 2);
    fit::apply(&mem_hash::f2, sp, 1, 2);

    fit::apply(&mem_hash::g2, x, 1, 2);
    fit::apply(&mem_hash::g2, rcx, 1, 2);
    fit::apply(&mem_hash::g2, &x, 1, 2);
    fit::apply(&mem_hash::g2, pcx, 1, 2);
    fit::apply(&mem_hash::g2, sp, 1, 2);

    fit::apply(&mem_hash::f3, x, 1, 2, 3);
    fit::apply(&mem_hash::f3, &x, 1, 2, 3);
    fit::apply(&mem_hash::f3, sp, 1, 2, 3);

    fit::apply(&mem_hash::g3, x, 1, 2, 3);
    fit::apply(&mem_hash::g3, rcx, 1, 2, 3);
    fit::apply(&mem_hash::g3, &x, 1, 2, 3);
    fit::apply(&mem_hash::g3, pcx, 1, 2, 3);
    fit::apply(&mem_hash::g3, sp, 1, 2, 3);

    fit::apply(&mem_hash::f4, x, 1, 2, 3, 4);
    fit::apply(&mem_hash::f4, &x, 1, 2, 3, 4);
    fit::apply(&mem_hash::f4, sp, 1, 2, 3, 4);

    fit::apply(&mem_hash::g4, x, 1, 2, 3, 4);
    fit::apply(&mem_hash::g4, rcx, 1, 2, 3, 4);
    fit::apply(&mem_hash::g4, &x, 1, 2, 3, 4);
    fit::apply(&mem_hash::g4, pcx, 1, 2, 3, 4);
    fit::apply(&mem_hash::g4, sp, 1, 2, 3, 4);

    fit::apply(&mem_hash::f5, x, 1, 2, 3, 4, 5);
    fit::apply(&mem_hash::f5, &x, 1, 2, 3, 4, 5);
    fit::apply(&mem_hash::f5, sp, 1, 2, 3, 4, 5);

    fit::apply(&mem_hash::g5, x, 1, 2, 3, 4, 5);
    fit::apply(&mem_hash::g5, rcx, 1, 2, 3, 4, 5);
    fit::apply(&mem_hash::g5, &x, 1, 2, 3, 4, 5);
    fit::apply(&mem_hash::g5, pcx, 1, 2, 3, 4, 5);
    fit::apply(&mem_hash::g5, sp, 1, 2, 3, 4, 5);

    fit::apply(&mem_hash::f6, x, 1, 2, 3, 4, 5, 6);
    fit::apply(&mem_hash::f6, &x, 1, 2, 3, 4, 5, 6);
    fit::apply(&mem_hash::f6, sp, 1, 2, 3, 4, 5, 6);

    fit::apply(&mem_hash::g6, x, 1, 2, 3, 4, 5, 6);
    fit::apply(&mem_hash::g6, rcx, 1, 2, 3, 4, 5, 6);
    fit::apply(&mem_hash::g6, &x, 1, 2, 3, 4, 5, 6);
    fit::apply(&mem_hash::g6, pcx, 1, 2, 3, 4, 5, 6);
    fit::apply(&mem_hash::g6, sp, 1, 2, 3, 4, 5, 6);

    fit::apply(&mem_hash::f7, x, 1, 2, 3, 4, 5, 6, 7);
    fit::apply(&mem_hash::f7, &x, 1, 2, 3, 4, 5, 6, 7);
    fit::apply(&mem_hash::f7, sp, 1, 2, 3, 4, 5, 6, 7);

    fit::apply(&mem_hash::g7, x, 1, 2, 3, 4, 5, 6, 7);
    fit::apply(&mem_hash::g7, rcx, 1, 2, 3, 4, 5, 6, 7);
    fit::apply(&mem_hash::g7, &x, 1, 2, 3, 4, 5, 6, 7);
    fit::apply(&mem_hash::g7, pcx, 1, 2, 3, 4, 5, 6, 7);
    fit::apply(&mem_hash::g7, sp, 1, 2, 3, 4, 5, 6, 7);

    fit::apply(&mem_hash::f8, x, 1, 2, 3, 4, 5, 6, 7, 8);
    fit::apply(&mem_hash::f8, &x, 1, 2, 3, 4, 5, 6, 7, 8);
    fit::apply(&mem_hash::f8, sp, 1, 2, 3, 4, 5, 6, 7, 8);

    fit::apply(&mem_hash::g8, x, 1, 2, 3, 4, 5, 6, 7, 8);
    fit::apply(&mem_hash::g8, rcx, 1, 2, 3, 4, 5, 6, 7, 8);
    fit::apply(&mem_hash::g8, &x, 1, 2, 3, 4, 5, 6, 7, 8);
    fit::apply(&mem_hash::g8, pcx, 1, 2, 3, 4, 5, 6, 7, 8);
    fit::apply(&mem_hash::g8, sp, 1, 2, 3, 4, 5, 6, 7, 8);

    FIT_TEST_CHECK(fit::apply(&mem_hash::hash, x) == 17610 && fit::apply(&mem_hash::hash, sp) == 2155);
}

struct hash_base
{
    mutable unsigned int hash;

    hash_base(): hash(0) {}

    int f0() { f1(17); return 0; }
    int g0() const { g1(17); return 0; }

    int f1(int a1) { hash = (hash * 17041 + a1) % 32768; return 0; }
    int g1(int a1) const { hash = (hash * 17041 + a1 * 2) % 32768; return 0; }

    int f2(int a1, int a2) { f1(a1); f1(a2); return 0; }
    int g2(int a1, int a2) const { g1(a1); g1(a2); return 0; }

    int f3(int a1, int a2, int a3) { f2(a1, a2); f1(a3); return 0; }
    int g3(int a1, int a2, int a3) const { g2(a1, a2); g1(a3); return 0; }

    int f4(int a1, int a2, int a3, int a4) { f3(a1, a2, a3); f1(a4); return 0; }
    int g4(int a1, int a2, int a3, int a4) const { g3(a1, a2, a3); g1(a4); return 0; }

    int f5(int a1, int a2, int a3, int a4, int a5) { f4(a1, a2, a3, a4); f1(a5); return 0; }
    int g5(int a1, int a2, int a3, int a4, int a5) const { g4(a1, a2, a3, a4); g1(a5); return 0; }

    int f6(int a1, int a2, int a3, int a4, int a5, int a6) { f5(a1, a2, a3, a4, a5); f1(a6); return 0; }
    int g6(int a1, int a2, int a3, int a4, int a5, int a6) const { g5(a1, a2, a3, a4, a5); g1(a6); return 0; }

    int f7(int a1, int a2, int a3, int a4, int a5, int a6, int a7) { f6(a1, a2, a3, a4, a5, a6); f1(a7); return 0; }
    int g7(int a1, int a2, int a3, int a4, int a5, int a6, int a7) const { g6(a1, a2, a3, a4, a5, a6); g1(a7); return 0; }

    int f8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) { f7(a1, a2, a3, a4, a5, a6, a7); f1(a8); return 0; }
    int g8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) const { g7(a1, a2, a3, a4, a5, a6, a7); g1(a8); return 0; }
};

struct derived_hash: public hash_base
{
};

FIT_TEST_CASE()
{

    derived_hash x;

    derived_hash const & rcx = x;
    derived_hash const * pcx = &x;

    std::shared_ptr<derived_hash> sp(new derived_hash);

    fit::apply(&derived_hash::f0, x);
    fit::apply(&derived_hash::f0, &x);
    fit::apply(&derived_hash::f0, sp);

    fit::apply(&derived_hash::g0, x);
    fit::apply(&derived_hash::g0, rcx);
    fit::apply(&derived_hash::g0, &x);
    fit::apply(&derived_hash::g0, pcx);
    fit::apply(&derived_hash::g0, sp);

    fit::apply(&derived_hash::f1, x, 1);
    fit::apply(&derived_hash::f1, &x, 1);
    fit::apply(&derived_hash::f1, sp, 1);

    fit::apply(&derived_hash::g1, x, 1);
    fit::apply(&derived_hash::g1, rcx, 1);
    fit::apply(&derived_hash::g1, &x, 1);
    fit::apply(&derived_hash::g1, pcx, 1);
    fit::apply(&derived_hash::g1, sp, 1);

    fit::apply(&derived_hash::f2, x, 1, 2);
    fit::apply(&derived_hash::f2, &x, 1, 2);
    fit::apply(&derived_hash::f2, sp, 1, 2);

    fit::apply(&derived_hash::g2, x, 1, 2);
    fit::apply(&derived_hash::g2, rcx, 1, 2);
    fit::apply(&derived_hash::g2, &x, 1, 2);
    fit::apply(&derived_hash::g2, pcx, 1, 2);
    fit::apply(&derived_hash::g2, sp, 1, 2);

    fit::apply(&derived_hash::f3, x, 1, 2, 3);
    fit::apply(&derived_hash::f3, &x, 1, 2, 3);
    fit::apply(&derived_hash::f3, sp, 1, 2, 3);

    fit::apply(&derived_hash::g3, x, 1, 2, 3);
    fit::apply(&derived_hash::g3, rcx, 1, 2, 3);
    fit::apply(&derived_hash::g3, &x, 1, 2, 3);
    fit::apply(&derived_hash::g3, pcx, 1, 2, 3);
    fit::apply(&derived_hash::g3, sp, 1, 2, 3);

    fit::apply(&derived_hash::f4, x, 1, 2, 3, 4);
    fit::apply(&derived_hash::f4, &x, 1, 2, 3, 4);
    fit::apply(&derived_hash::f4, sp, 1, 2, 3, 4);

    fit::apply(&derived_hash::g4, x, 1, 2, 3, 4);
    fit::apply(&derived_hash::g4, rcx, 1, 2, 3, 4);
    fit::apply(&derived_hash::g4, &x, 1, 2, 3, 4);
    fit::apply(&derived_hash::g4, pcx, 1, 2, 3, 4);
    fit::apply(&derived_hash::g4, sp, 1, 2, 3, 4);

    fit::apply(&derived_hash::f5, x, 1, 2, 3, 4, 5);
    fit::apply(&derived_hash::f5, &x, 1, 2, 3, 4, 5);
    fit::apply(&derived_hash::f5, sp, 1, 2, 3, 4, 5);

    fit::apply(&derived_hash::g5, x, 1, 2, 3, 4, 5);
    fit::apply(&derived_hash::g5, rcx, 1, 2, 3, 4, 5);
    fit::apply(&derived_hash::g5, &x, 1, 2, 3, 4, 5);
    fit::apply(&derived_hash::g5, pcx, 1, 2, 3, 4, 5);
    fit::apply(&derived_hash::g5, sp, 1, 2, 3, 4, 5);

    fit::apply(&derived_hash::f6, x, 1, 2, 3, 4, 5, 6);
    fit::apply(&derived_hash::f6, &x, 1, 2, 3, 4, 5, 6);
    fit::apply(&derived_hash::f6, sp, 1, 2, 3, 4, 5, 6);

    fit::apply(&derived_hash::g6, x, 1, 2, 3, 4, 5, 6);
    fit::apply(&derived_hash::g6, rcx, 1, 2, 3, 4, 5, 6);
    fit::apply(&derived_hash::g6, &x, 1, 2, 3, 4, 5, 6);
    fit::apply(&derived_hash::g6, pcx, 1, 2, 3, 4, 5, 6);
    fit::apply(&derived_hash::g6, sp, 1, 2, 3, 4, 5, 6);

    fit::apply(&derived_hash::f7, x, 1, 2, 3, 4, 5, 6, 7);
    fit::apply(&derived_hash::f7, &x, 1, 2, 3, 4, 5, 6, 7);
    fit::apply(&derived_hash::f7, sp, 1, 2, 3, 4, 5, 6, 7);

    fit::apply(&derived_hash::g7, x, 1, 2, 3, 4, 5, 6, 7);
    fit::apply(&derived_hash::g7, rcx, 1, 2, 3, 4, 5, 6, 7);
    fit::apply(&derived_hash::g7, &x, 1, 2, 3, 4, 5, 6, 7);
    fit::apply(&derived_hash::g7, pcx, 1, 2, 3, 4, 5, 6, 7);
    fit::apply(&derived_hash::g7, sp, 1, 2, 3, 4, 5, 6, 7);

    fit::apply(&derived_hash::f8, x, 1, 2, 3, 4, 5, 6, 7, 8);
    fit::apply(&derived_hash::f8, &x, 1, 2, 3, 4, 5, 6, 7, 8);
    fit::apply(&derived_hash::f8, sp, 1, 2, 3, 4, 5, 6, 7, 8);

    fit::apply(&derived_hash::g8, x, 1, 2, 3, 4, 5, 6, 7, 8);
    fit::apply(&derived_hash::g8, rcx, 1, 2, 3, 4, 5, 6, 7, 8);
    fit::apply(&derived_hash::g8, &x, 1, 2, 3, 4, 5, 6, 7, 8);
    fit::apply(&derived_hash::g8, pcx, 1, 2, 3, 4, 5, 6, 7, 8);
    fit::apply(&derived_hash::g8, sp, 1, 2, 3, 4, 5, 6, 7, 8);

    FIT_TEST_CHECK(fit::apply(&derived_hash::hash, x) == 17610 && fit::apply(&derived_hash::hash, sp) == 2155);
}

struct dm_t
{
    int m;
};

FIT_TEST_CASE()
{
    dm_t x = { 0 };

    fit::apply( &dm_t::m, x ) = 401;

    FIT_TEST_CHECK( x.m == 401 );
    FIT_TEST_CHECK( fit::apply( &dm_t::m, x ) == 401 );

    fit::apply( &dm_t::m, &x ) = 502;

    FIT_TEST_CHECK( x.m == 502 );
    FIT_TEST_CHECK( fit::apply( &dm_t::m, &x ) == 502 );

    dm_t * px = &x;

    fit::apply( &dm_t::m, px ) = 603;

    FIT_TEST_CHECK( x.m == 603 );
    FIT_TEST_CHECK( fit::apply( &dm_t::m, px ) == 603 );

    dm_t const & cx = x;
    dm_t const * pcx = &x;

    FIT_TEST_CHECK( fit::apply( &dm_t::m, cx ) == 603 );
    FIT_TEST_CHECK( fit::apply( &dm_t::m, pcx ) == 603 );

}
