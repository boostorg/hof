#ifndef GUARD_TEST_H
#define GUARD_TEST_H

#include <type_traits>
#include <tuple>
#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include <boost/fit/detail/forward.hpp>

#ifndef BOOST_FIT_HAS_STATIC_TEST_CHECK
#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7) || defined(_MSC_VER)
#define BOOST_FIT_HAS_STATIC_TEST_CHECK 0
#else
#define BOOST_FIT_HAS_STATIC_TEST_CHECK 1
#endif
#endif

#ifndef BOOST_FIT_HAS_GENERIC_LAMBDA
#ifdef __clang__
#if __has_feature(cxx_generic_lambdas)
#define BOOST_FIT_HAS_GENERIC_LAMBDA 1
#else
#define BOOST_FIT_HAS_GENERIC_LAMBDA 0
#endif
#else
#define BOOST_FIT_HAS_GENERIC_LAMBDA 0
#endif


#endif

#define BOOST_FIT_PP_CAT(x, y) BOOST_FIT_PP_PRIMITIVE_CAT(x, y)
#define BOOST_FIT_PP_PRIMITIVE_CAT(x, y) x ## y

namespace boost { namespace fit { namespace test {

typedef std::function<void()> test_case;
static std::vector<test_case> test_cases;

struct auto_register
{
    auto_register(test_case tc)
    {
        test_cases.push_back(tc);
    }
};

#define BOOST_FIT_DETAIL_TEST_CASE(name) \
struct name \
{ void operator()() const; }; \
static boost::fit::test::auto_register BOOST_FIT_PP_CAT(name, _register) = boost::fit::test::auto_register(name()); \
void name::operator()() const

template<class T>
T bare(const T&);

template<class T>
inline void unused(T&&) {}

}}} // namespace boost::fit

#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define BOOST_FIT_STATIC_AUTO constexpr auto
#else
#define BOOST_FIT_STATIC_AUTO const constexpr auto
#endif

#define STATIC_ASSERT_SAME(...) static_assert(std::is_same<__VA_ARGS__>::value, "Types are not the same")
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define STATIC_ASSERT_MOVE_ONLY(T)
#else
#define STATIC_ASSERT_MOVE_ONLY(T) static_assert(!std::is_copy_constructible<T>::value && std::is_move_constructible<T>::value, "Not movable")
#endif
#if defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
#define STATIC_ASSERT_NOT_DEFAULT_CONSTRUCTIBLE(T)
#else
#define STATIC_ASSERT_NOT_DEFAULT_CONSTRUCTIBLE(T) static_assert(!std::is_default_constructible<T>::value, "Default constructible")
#endif
#define STATIC_ASSERT_EMPTY(x) static_assert(std::is_empty<decltype(boost::fit::test::bare(x))>::value, "Not empty");


#define BOOST_FIT_TEST_CASE() BOOST_FIT_DETAIL_TEST_CASE(BOOST_FIT_PP_CAT(test_, __LINE__))
#define BOOST_FIT_STATIC_TEST_CASE() struct BOOST_FIT_PP_CAT(test_, __LINE__)

#define BOOST_FIT_TEST_TEMPLATE(...) typedef std::integral_constant<int, sizeof(__VA_ARGS__)> BOOST_FIT_PP_CAT(test_template_, __LINE__)

#define BOOST_FIT_TEST_CHECK(...) if (!(__VA_ARGS__)) std::cout << "*****FAILED: " << #__VA_ARGS__ << "@" << __FILE__ << ": " << __LINE__ << std::endl
#define BOOST_FIT_STRINGIZE(...) #__VA_ARGS__

#if BOOST_FIT_HAS_STATIC_TEST_CHECK
#define BOOST_FIT_STATIC_TEST_CHECK(...) static_assert(__VA_ARGS__, BOOST_FIT_STRINGIZE(__VA_ARGS__))
#else
#define BOOST_FIT_STATIC_TEST_CHECK(...)
#endif
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
    T operator()(T & x, U y) const
    {
        return x+=y;
    }

};

struct unary_class
{
    template<class T>
    constexpr T&& operator()(T&& x) const
    {
        return boost::fit::forward<T>(x);
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
    // Note: Taking the tuple by value causes the compiler to ICE on gcc 4.7
    // when called in a constexpr context.
    template<class T>
    constexpr int operator()(const T& t) const
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
	for(const auto& tc: boost::fit::test::test_cases) tc();
    return 0;
}
 
#endif
