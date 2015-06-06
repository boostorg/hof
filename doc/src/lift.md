lift
====

Description
-----------

The `lift` operator will wrap a template function in a function object so it
can be passed to higher-order functions. The `FIT_LIFT` macro will wrap the
function using a generic lambda. As such, it will not preserve `constexpr`.
The `FIT_LIFT_CLASS` can be used to declare a class that will wrap function.
This will preserve `constexpr` and it can be used on older compilers that
don't support generic lambdas yet.

Synopsis
--------

    // Wrap the function in a generic lambda
    #define FIT_LIFT(...)

    // Declare a class named `name` that will forward to the function
    #define FIT_LIFT_CLASS(name, ...)

Example
-------

    auto my_max = FIT_LIFT(std::max);
    assert(my_max(3, 4) == std::max(3, 4));
    // Declare the class `max_f`
    FIT_LIFT_CLASS(max_f, std::max);
    assert(max_f()(3, 4) == std::max(3, 4));

