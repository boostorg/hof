/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    print.cpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "example.h"
#include <tuple>

using namespace fit;

FIT_STATIC_LAMBDA_FUNCTION(tuple_transform) = [](auto&& sequence, auto f)
{
    return unpack(by(f, construct<std::tuple>()))(std::forward<decltype(sequence)>(sequence));
};

FIT_STATIC_LAMBDA_FUNCTION(tuple_for_each) = [](auto&& sequence, auto f)
{
    return unpack(by(f))(std::forward<decltype(sequence)>(sequence));
};

FIT_STATIC_LAMBDA_FUNCTION(tuple_fold) = [](auto&& sequence, auto f)
{
    return unpack(compress(f))(std::forward<decltype(sequence)>(sequence));
};

FIT_STATIC_FUNCTION(tuple_cat) = unpack(construct<std::tuple>());

FIT_STATIC_FUNCTION(tuple_join) = unpack(tuple_cat);

FIT_STATIC_LAMBDA_FUNCTION(tuple_filter) = [](auto&& sequence, auto predicate)
{
    return compose(tuple_join, tuple_transform)(
        std::forward<decltype(sequence)>(sequence),
        [&](auto&& x)
        {
            return conditional(
                if_(predicate(std::forward<decltype(x)>(x)))(pack),
                always(pack())
            )(std::forward<decltype(x)>(x));
        }
    );
};

FIT_STATIC_LAMBDA_FUNCTION(tuple_zip_with) = [](auto&& sequence1, auto&& sequence2, auto f)
{
    auto&& functions = tuple_transform(
        std::forward<decltype(sequence1)>(sequence1),
        [&](auto&& x)
        {
            return [&](auto&& y)
            {
                return f(std::forward<decltype(x)>(x), std::forward<decltype(y)>(y));
            };
        }
    );
    auto combined = unpack(capture(construct<std::tuple>())(combine))(functions);
    return unpack(combined)(std::forward<decltype(sequence2)>(sequence2));
};

FIT_STATIC_LAMBDA_FUNCTION(tuple_dot) = [](auto&& a, auto&& b)
{
    auto product = tuple_zip_with(a, b, [](auto x, auto y) { return x*y; });
    return tuple_fold(product, [](auto x, auto y) { return x+y; });
};

void test_tranform()
{
    auto t = std::make_tuple(1, 2);
    auto r = tuple_transform(t, [](int i) { return i*i; });
    assert(r == std::make_tuple(1, 4));
    (void)r;
}

void test_filter()
{
    auto t = std::make_tuple(1, 2, 'x', 3);
    auto r = tuple_filter(t, [](auto x) { return std::is_same<int, decltype(x)>(); });
    assert(r == std::make_tuple(1, 2, 3));
    (void)r;
}

void test_zip()
{
    auto t1 = std::make_tuple(1, 2);
    auto t2 = std::make_tuple(3, 4);
    auto p = tuple_zip_with(t1, t2, [](auto x, auto y) { return x*y; });
    int r = tuple_fold(p, [](auto x, auto y) { return x+y; });
    assert(r == (1*3 + 4*2));
    (void)r;
}

void test_dot()
{
    auto t1 = std::make_tuple(1, 2);
    auto t2 = std::make_tuple(3, 4);
    int r = tuple_dot(t1, t2);
    assert(r == (1*3 + 4*2));
    (void)r;
}

int main() 
{
    test_tranform();
    test_filter();
    test_zip();
}

