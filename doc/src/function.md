FIT_STATIC_FUNCTION
===================

Description
-----------

The `FIT_STATIC_FUNCTION` macro allows initializing a function object from a
`constexpr` expression. It also ensures that the function object will have a
unique address across translation units. This helps to avoid ODR violations.

Example
-------

    FIT_STATIC_FUNCTION(sum) = fit::partial([](int x, int y)
    {
        return x + y;
    });

