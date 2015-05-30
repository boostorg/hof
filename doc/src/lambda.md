FIT_STATIC_LAMBDA
=================

Description
-----------

The `FIT_STATIC_LAMBDA` macro allows initializing non-capturing lambdas at
compile-time in a `constexpr` expression.

Example
-------

    const constexpr auto add_one = FIT_STATIC_LAMBDA(int x)
    {
        return x + 1;
    };

FIT_STATIC_LAMBDA_FUNCTION
==========================

Description
-----------

The `FIT_STATIC_LAMBDA_FUNCTION` macro allows initializes a global function
object that contains non-capturing lambdas. It also ensures that the global
function object has a unique address across translation units. This helps
prevent possible ODR-violations.

Example
-------

    FIT_STATIC_LAMBDA_FUNCTION(add_one) = [](int x)
    {
        return x + 1;
    };

