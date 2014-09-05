FIT_STATIC_LAMBDA
=================

Description
-----------

The `FIT_STATIC_LAMBDA` macro allows initializing lambdas at compile-time
in a `constexpr` expression.

Example
-------

    const constexpr auto add_one = FIT_STATIC_LAMBDA(int x)
    {
        return x + 1;
    };

