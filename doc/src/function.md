FIT_STATIC_FUNCTION
===================

Description
-----------

The `FIT_STATIC_FUNCTION` macro allows initializing a function object from
lambdas and adaptors at compile-time in a `constexpr` expression.

Example
-------

    FIT_STATIC_FUNCTION(sum) = fit::partial([](int x, int y)
    {
        return x + y;
    });

