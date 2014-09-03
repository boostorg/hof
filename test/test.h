#ifndef GUARD_TEST_H
#define GUARD_TEST_H

#include <type_traits>
#include <tuple>
#include <iostream>
#include <functional>
#include <vector>
#include <memory>

#define FIT_PP_CAT(x, y) FIT_PP_PRIMITIVE_CAT(x, y)
#define FIT_PP_PRIMITIVE_CAT(x, y) x ## y

namespace fit { namespace test {

typedef std::function<void()> test_case;
static std::vector<test_case> test_cases;

struct auto_register
{
    auto_register(test_case tc)
    {
        test_cases.push_back(tc);
    }
};

#define FIT_DETAIL_TEST_CASE(name) \
struct name \
{ void operator()() const; }; \
static fit::test::auto_register FIT_PP_CAT(name, _register) = fit::test::auto_register(name()); \
void name::operator()() const

}}


#define STATIC_ASSERT_SAME(...) static_assert(std::is_same<__VA_ARGS__>::value, "Types are not the same")
#define STATIC_ASSERT_MOVE_ONLY(T) static_assert(!std::is_copy_constructible<T>::value && std::is_move_constructible<T>::value, "Not movable")
#define FIT_TEST_CASE() FIT_DETAIL_TEST_CASE(FIT_PP_CAT(test_, __LINE__))
#define FIT_STATIC_TEST_CASE() struct FIT_PP_CAT(test_, __LINE__)

#define FIT_TEST_TEMPLATE(...) typedef std::integral_constant<int, sizeof(__VA_ARGS__)> FIT_PP_CAT(test_template_, __LINE__)

#define FIT_TEST_CHECK(...) if (!(__VA_ARGS__)) std::cout << "*****FAILED: " << #__VA_ARGS__ << "@" << __FILE__ << ": " << __LINE__ << std::endl

struct binary_class
{
    template<class T, class U>
    constexpr T operator()(T x, U y) const
    {
        return x+y;
    }

};

// typedef zen::defer_adaptor<binary_class_d> binary_class;

struct mutable_class
{
    template<class F>
    struct result;

    template<class F, class T, class U>
    struct result<F(T&, U)>
    {
        typedef T type;
    };

    template<class T, class U>
    constexpr T operator()(T & x, U y) const
    {
        return x+=y;
    }

};

struct unary_class
{
    template<class T>
    constexpr T&& operator()(T&& x) const
    {
        return std::forward<T>(x);
    }

};

struct void_class
{
    template<class T>
    constexpr void operator()(T) const
    {
    }
};

struct mono_class
{
    constexpr int operator()(int x) const
    {
        return x+1;
    }
};

struct tuple_class
{
    template<class T>
    constexpr int operator()(T t) const
    {
        return std::get<0>(t) + 1;
    }
};

template<class R>
struct explicit_class
{
    template<class T>
    R operator()(T x)
    {
        return static_cast<R>(x);
    }
};

struct move_class
{
    std::unique_ptr<int> i;
    move_class() : i(new int(0))
    {}

    template<class T, class U>
    constexpr T operator()(T x, U y) const
    {
        return x+y+*i;
    }
};

int main()
{
	for(const auto& tc: fit::test::test_cases) tc();
    return 0;
}
 
#endif