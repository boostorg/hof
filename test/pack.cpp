#include <fit/pack.hpp>
#include <fit/always.hpp>
#include <fit/identity.hpp>
#include <memory>
#include "test.hpp"

FIT_TEST_CASE()
{
    auto p1 = fit::pack_basic(1, 2);
    auto p2 = p1;
    FIT_TEST_CHECK(p2(binary_class()) == p1(binary_class()));
    
    FIT_STATIC_TEST_CHECK(fit::pack_basic(1, 2)(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_basic(1, 2)(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack(1, 2)(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack(1, 2)(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_forward(1, 2)(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_forward(1, 2)(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    int i = 1;
    static_assert(noexcept(fit::pack(1, 2)(fit::always())), "noexcept pack");
    static_assert(noexcept(fit::pack_forward(i, i)(fit::always())), "noexcept pack");
    static_assert(noexcept(fit::pack_forward(1, 2)(fit::always())), "noexcept pack");
    static_assert(noexcept(fit::pack_basic(i, i)(fit::always())), "noexcept pack");
    static_assert(noexcept(fit::pack_basic(1, 2)(fit::always())), "noexcept pack");

    static_assert(noexcept(fit::pack()(fit::always())), "noexcept pack");
    static_assert(noexcept(fit::pack_forward()(fit::always())), "noexcept pack");
    static_assert(noexcept(fit::pack_basic()(fit::always())), "noexcept pack");
}

FIT_TEST_CASE()
{
    static_assert(noexcept(fit::pack_join(fit::pack(), fit::pack())(fit::always())), "noexcept pack");
    static_assert(noexcept(fit::pack_join(fit::pack(1), fit::pack(1))(fit::always())), "noexcept pack");
    auto p = fit::pack(1);
    static_assert(noexcept(fit::pack_join(p, fit::pack())(fit::always())), "noexcept pack");
    static_assert(noexcept(fit::pack_join(fit::pack(), p)(fit::always())), "noexcept pack");

}

FIT_TEST_CASE()
{
    static constexpr int x = 1;
    static constexpr int y = 2;

    auto p1 = fit::pack_basic(x, y);
    static_assert(!fit::detail::is_default_constructible<decltype(p1)>::value, "Pack default constructible");
    
    FIT_STATIC_TEST_CHECK(fit::pack_basic(x, y)(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_basic(x, y)(binary_class()) == 3 );

    auto p2 = fit::pack(std::ref(x), std::ref(y));
    static_assert(!fit::detail::is_default_constructible<decltype(p2)>::value, "Pack default constructible");

    FIT_STATIC_TEST_CHECK(fit::pack(x, y)(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack(std::ref(x), std::ref(y))(binary_class()) == 3 );

    auto p3 = fit::pack_forward(x, y);
    static_assert(!fit::detail::is_default_constructible<decltype(p3)>::value, "Pack default constructible");

    FIT_STATIC_TEST_CHECK(fit::pack_forward(x, y)(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_forward(x, y)(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::pack_basic()(fit::always(3)) == 3);
    FIT_TEST_CHECK(fit::pack_basic()(fit::always(3)) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::pack_basic(3)(fit::identity) == 3);
    FIT_TEST_CHECK(fit::pack_basic(3)(fit::identity) == 3 );
}

FIT_TEST_CASE()
{
    auto p = fit::pack(1);
    FIT_TEST_CHECK(fit::pack_join(p, fit::pack(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(p, p)(binary_class()) == 2);
    
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_basic(1), fit::pack_basic(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_basic(1), fit::pack_basic(2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack(1), fit::pack(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack(1), fit::pack(2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_forward(1), fit::pack_forward(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(1), fit::pack_forward(2))(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_basic(), fit::pack_basic(1, 2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_basic(), fit::pack_basic(1, 2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack(), fit::pack(1, 2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack(), fit::pack(1, 2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_forward(), fit::pack_forward(1, 2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(), fit::pack_forward(1, 2))(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_basic(1, 2), fit::pack_basic())(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_basic(1, 2), fit::pack_basic())(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack(1, 2), fit::pack())(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack(1, 2), fit::pack())(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_forward(1, 2), fit::pack_forward())(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(1, 2), fit::pack_forward())(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_basic(1), fit::pack_basic(), fit::pack_basic(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_basic(1), fit::pack_basic(), fit::pack_basic(2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack(1), fit::pack(), fit::pack(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack(1), fit::pack(), fit::pack(2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_forward(1), fit::pack_forward(), fit::pack_forward(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(1), fit::pack_forward(), fit::pack_forward(2))(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_basic(), fit::pack_basic(1), fit::pack_basic(), fit::pack_basic(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_basic(), fit::pack_basic(1), fit::pack_basic(), fit::pack_basic(2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack(), fit::pack(1), fit::pack(), fit::pack(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack(), fit::pack(1), fit::pack(), fit::pack(2))(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_forward(), fit::pack_forward(1), fit::pack_forward(), fit::pack_forward(2))(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(), fit::pack_forward(1), fit::pack_forward(), fit::pack_forward(2))(binary_class()) == 3 );
}

FIT_TEST_CASE()
{
    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_basic(1), fit::pack_basic(), fit::pack_basic(2), fit::pack_basic())(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_basic(1), fit::pack_basic(), fit::pack_basic(2), fit::pack_basic())(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack(1), fit::pack(), fit::pack(2), fit::pack())(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack(1), fit::pack(), fit::pack(2), fit::pack())(binary_class()) == 3 );

    FIT_STATIC_TEST_CHECK(fit::pack_join(fit::pack_forward(1), fit::pack_forward(), fit::pack_forward(2), fit::pack_forward())(binary_class()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(1), fit::pack_forward(), fit::pack_forward(2), fit::pack_forward())(binary_class()) == 3 );
}

struct deref
{
    int operator()(const std::unique_ptr<int>& i) const
    {
        return *i;
    }
};

FIT_TEST_CASE()
{
    std::unique_ptr<int> i(new int(3));
    FIT_TEST_CHECK(fit::pack_basic(i)(deref()) == 3);
    FIT_TEST_CHECK(fit::pack_basic(std::unique_ptr<int>(new int(3)))(deref()) == 3);
    FIT_TEST_CHECK(fit::pack_forward(std::unique_ptr<int>(new int(3)))(deref()) == 3);
    FIT_TEST_CHECK(fit::pack(std::unique_ptr<int>(new int(3)))(deref()) == 3);
    auto p = fit::pack_basic(std::unique_ptr<int>(new int(3)));
    FIT_TEST_CHECK(p(deref()) == 3);

    FIT_TEST_CHECK(fit::pack_join(fit::pack_basic(), fit::pack_basic(std::unique_ptr<int>(new int(3))))(deref()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack_forward(), fit::pack_forward(std::unique_ptr<int>(new int(3))))(deref()) == 3);
    FIT_TEST_CHECK(fit::pack_join(fit::pack(), fit::pack(std::unique_ptr<int>(new int(3))))(deref()) == 3);
    // FIT_TEST_CHECK(p(deref()) == 3);
}

struct move_rvalue 
{
    void operator()(std::string&& s) const 
    {
        std::string ss = std::move(s);
        FIT_TEST_CHECK(ss == "abcdef");
        s = "00000";
    }
};

struct check_rvalue 
{
    void operator()(std::string&& s) const 
    {
        FIT_TEST_CHECK(s == "abcdef");
    }
};

FIT_TEST_CASE()
{
    auto p = fit::pack_basic(std::string{"abcdef"});
    p(move_rvalue{});
    p(check_rvalue{});
}

FIT_TEST_CASE()
{
    auto p = fit::pack(std::string{"abcdef"});
    p(move_rvalue{});
    p(check_rvalue{});
}

struct empty1
{};

struct empty2
{};

FIT_TEST_CASE()
{
    static_assert(fit::detail::is_default_constructible<empty1, empty2>::value, "Not default constructible");
}

FIT_TEST_CASE()
{
    static constexpr auto p = fit::pack_basic(empty1());
    FIT_TEST_CHECK(p(fit::always(0)) == 0);
    FIT_STATIC_TEST_CHECK(p(fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");

}

FIT_TEST_CASE()
{
    static constexpr auto p = fit::pack_basic(empty1(), empty2());
    FIT_TEST_CHECK(p(fit::always(0)) == 0);
    FIT_STATIC_TEST_CHECK(p(fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");
}

FIT_TEST_CASE()
{
    static constexpr auto p = fit::pack_basic(fit::pack_basic(), fit::pack_basic());
    FIT_TEST_CHECK(p(fit::always(0)) == 0);
    FIT_STATIC_TEST_CHECK(p(fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");
}

FIT_TEST_CASE()
{
    static constexpr auto p = fit::pack_basic(empty1(), empty2(), empty1());
    FIT_TEST_CHECK(p(fit::always(0)) == 0);
    FIT_STATIC_TEST_CHECK(p(fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");
}

FIT_TEST_CASE()
{
    static constexpr auto p = fit::pack_basic(empty1(), fit::pack_basic(empty1(), empty2()));
    FIT_TEST_CHECK(p(fit::always(0)) == 0);
    FIT_STATIC_TEST_CHECK(p(fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");
}

FIT_TEST_CASE()
{
    static constexpr auto p = fit::pack_basic(fit::pack_basic(), fit::pack_basic(fit::pack_basic()), empty1(), fit::pack_basic(empty1(), empty2()));
    FIT_TEST_CHECK(p(fit::always(0)) == 0);
    FIT_STATIC_TEST_CHECK(p(fit::always(0)) == 0);
#ifndef _MSC_VER
    static_assert(std::is_empty<decltype(p)>::value, "Pack not empty");
#endif
    static_assert(fit::detail::is_default_constructible<decltype(p)>::value, "Not default constructible");
}

struct not_default_constructible
{
    int i;
    constexpr not_default_constructible(int x) : i(x)
    {}
};

struct select_i
{
    template<class T>
    constexpr int operator()(T&& x) const
    {
        return x.i;
    } 

    template<class T, class U>
    constexpr int operator()(T&& x, U&& y) const
    {
        return x.i + y.i;
    } 

    template<class T, class U, class V>
    constexpr int operator()(T&& x, U&& y, V&& z) const
    {
        return x.i + y.i + z.i;
    } 
};

FIT_TEST_CASE()
{
    static_assert(!fit::detail::is_default_constructible<not_default_constructible>::value, "Default constructible");
    auto p = fit::pack_basic(not_default_constructible(3));
    static_assert(!fit::detail::is_default_constructible<decltype(p)>::value, "Pack default constructible");
    auto p1 = fit::pack_forward(p);
    static_assert(!fit::detail::is_default_constructible<decltype(p1)>::value, "Packs default constructible");
    auto p2 = fit::pack_forward(p, p);
    static_assert(!fit::detail::is_default_constructible<decltype(p2)>::value, "Packs default constructible");
    auto p3 = fit::pack_forward(p, p, p);
    static_assert(!fit::detail::is_default_constructible<decltype(p3)>::value, "Packs default constructible");
    FIT_TEST_CHECK(p(select_i()) == 3);
    FIT_STATIC_TEST_CHECK(fit::pack_basic(not_default_constructible(3))(select_i()) == 3);
}

FIT_TEST_CASE()
{
    static_assert(!fit::detail::is_default_constructible<not_default_constructible>::value, "Default constructible");
    auto p = fit::pack_basic(not_default_constructible(1), not_default_constructible(2));
    static_assert(!fit::detail::is_default_constructible<decltype(p)>::value, "Pack default constructible");
    auto p1 = fit::pack_forward(p);
    static_assert(!fit::detail::is_default_constructible<decltype(p1)>::value, "Packs default constructible");
    auto p2 = fit::pack_forward(p, p);
    static_assert(!fit::detail::is_default_constructible<decltype(p2)>::value, "Packs default constructible");
    auto p3 = fit::pack_forward(p, p, p);
    static_assert(!fit::detail::is_default_constructible<decltype(p3)>::value, "Packs default constructible");
    FIT_TEST_CHECK(p(select_i()) == 3);
    FIT_STATIC_TEST_CHECK(fit::pack_basic(not_default_constructible(1), not_default_constructible(2))(select_i()) == 3);
}

FIT_TEST_CASE()
{
    static_assert(!fit::detail::is_default_constructible<not_default_constructible>::value, "Default constructible");
    auto p = fit::pack_basic(not_default_constructible(1), not_default_constructible(1), not_default_constructible(1));
    static_assert(!fit::detail::is_default_constructible<decltype(p)>::value, "Pack default constructible");
    auto p1 = fit::pack_forward(p);
    static_assert(!fit::detail::is_default_constructible<decltype(p1)>::value, "Packs default constructible");
    auto p2 = fit::pack_forward(p, p);
    static_assert(!fit::detail::is_default_constructible<decltype(p2)>::value, "Packs default constructible");
    auto p3 = fit::pack_forward(p, p, p);
    static_assert(!fit::detail::is_default_constructible<decltype(p3)>::value, "Packs default constructible");
    FIT_TEST_CHECK(p(select_i()) == 3);
    FIT_STATIC_TEST_CHECK(fit::pack_basic(not_default_constructible(1), not_default_constructible(1), not_default_constructible(1))(select_i()) == 3);
}


