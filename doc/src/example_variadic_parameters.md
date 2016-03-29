Variadic parameters
===================

The library provides utilities to simplify handling variadic parameters. For example, if we want to write a variadic print function that prints each argument, like this:

    print("Hello", "World");

We can write this simply by using the [`by`](by.md) adaptor:

    FIT_STATIC_FUNCTION(print) = by(std::cout << _);

The [`by`](by.md) adaptor calls the function for each argument passed in. In this case, we write `std::cout << _` which uses the [placeholders](placeholders.md) to create a function that print to `std::cout`.  

We can also take binary functions and turn them easily into variadic functions
using [`compress`](compress.md), which will do a fold over the variadic parameters. So a variadic `max` function could be written like
this:

    FIT_STATIC_FUNCTION(max) = compress(FIT_LIFT(std::max));

[`FIT_LIFT`](lift.md) is used to grab the entire overload set of `std::max` function, which is needed since `std::max` is templated and we want the variadic `std::max` function to handle any types as well. So now it can be called like this:

    auto n = max(1, 2, 4, 3); // Returns 4
    auto m = max(0.1, 0.2, 0.5, 0.4); // Returns 0.5

By using [`compress`](compress.md), `max(1, 2, 4, 3)` is called like `std::max(std::max(std::max(1, 2), 4), 3)` and `max(0.1, 0.2, 0.5, 0.4)` is called like `std::max(std::max(std::max(0.1, 0.2), 0.5), 0.4)`.

