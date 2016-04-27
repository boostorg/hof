Getting started
===============

Function Objects
----------------

The Fit library can work with all different types of functions in C++, including static functions, member functions, lambdas, and function objects. To refer to the function a function pointer(or member function pointer) can be used. However, a function pointer can only refer to one function in an overload set of functions, and it requires explicit casting to select that overload. For example, if we had a templated `sum` function, we would need an explicit cast to pass that function to the `std::accumulate`: 

    template<class T, class U>
    auto sum(T x, U y)
    {
        return x + y;
    }

    auto sum_int = (int (*)(int, int))&sum;
    // Call integer overload
    int i = sum_int(1, 2);
    // Or pass to an algorithm
    std::vector<int> v = { 1, 2, 3 };
    int total = std::accumulate(v.begin(), v.end(), 0, sum_int);


Instead, a function object allows the ability to encapsulate an entire overload set into one object. This can be done by defining a class that overrides the call operator like this:

    // A sum function object
    struct sum_f
    {
        template<class T, class U>
        auto operator()(T x, U y) const
        {
            return x + y;
        }
    };

There are few things to note about this. First, the call operator member function is always declared `const`, which is generally required to be used with Fit.(Note: The [`mutable_`](mutable.md) adaptor can be used to make a mutable function object have a `const` call operator, but this should generally be avoided). Secondly, the `sum_f` class must be constructed first before it can be called:

    auto sum = sum_f();
    // Call sum function
    auto three = sum(1, 2);
    // Or pass to an algorithm
    std::vector<int> v = { 1, 2, 3 };
    int total = std::accumulate(v.begin(), v.end(), 0, sum);

Because the function is templated, it can be called on any type that has the plus `+` operator, not just integers. Futhermore, the `sum` variable can be used to refer to the entire overload set.

Lifting functions
-----------------

Another alternative to defining a function object, is to lift the templated function using [`FIT_LIFT`](lift.md). This will turn the entire overload set into one object like a function object:

    template<class T, class U>
    auto sum(T x, U y)
    {
        return x + y;
    }

    // Pass sum to an algorithm
    std::vector<int> v = { 1, 2, 3 };
    int total = std::accumulate(v.begin(), v.end(), 0, FIT_LIFT(sum));

However, due to limitations in C++14 this will not preserve `constexpr`. In those cases, its better to use a function object.

Declaring functions
-------------------

Now, this is useful for local functions. However, many times we want to write functions and make them available for others to use. The Fit library provides [`FIT_STATIC_FUNCTION`](function.md) to declare the function object at the global or namespace scope:

    FIT_STATIC_FUNCTION(sum) = sum_f();

The [`FIT_STATIC_FUNCTION`](function.md) declares a global variable following the best practices as outlined in [N4381](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4381.html). This includes compile-time initialization of the function object to avoid the [static initialization order fiasco](https://isocpp.org/wiki/faq/ctors#static-init-order) and an external address of the function object that is the same across translation units to avoid possible One-Definition-Rule(ODR) violations. In C++14, this can also be achieved by using a reference to a template variable:

    template <class T>
    constexpr T static_function{};

    static constexpr auto&& sum = static_function<sum_f>;

The [`FIT_STATIC_FUNCTION`](function.md) macro provides a portable way to do this that supports C++11 and MSVC, and allows supporting inline variables(in [N4424](http://open-std.org/JTC1/SC22/WG21/docs/papers/2015/n4424.pdf)) in future versions of C++.

Adaptors
--------

Now we have defined the function as a function object, we can add new "enhancements" to the function. One enhancement is to write "extension" methods. The proposal [N4165](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4165.pdf) for Unified Call Syntax(UFCS) in C++17 would have allowed a function call of `x.f(y)` to become `f(x, y)`. Without UFCS in C++, we can instead use pipable function which would transform `x | f(y)` into `f(x, y)`. To make `sum_f` function pipable using the [`pipable`](pipable.md) adaptor, we can simply write:

    FIT_STATIC_FUNCTION(sum) = pipable(sum_f());

Then the parameters can be piped into it, like this:

    auto three = 1 | sum(2);

Pipable function can be chained mutliple times just like the `.` operator:

    auto four = 1 | sum(2) | sum(1);

Alternatively, instead of using the `|` operator, pipable functions can be chained together using the [`flow`](flow.md) adaptor:

    auto four = flow(sum(2), sum(1))(1); 

Another enhancement that can be done to functions is defining named infix operators using the [`infix`](infix.md) adaptor:

    FIT_STATIC_FUNCTION(sum) = infix(sum_f());

And it could be called like this:

    auto three = 1 <sum> 2;

In addition, adaptors are provided that support simple functional operations such as [partial application](https://en.wikipedia.org/wiki/Partial_application) and [function composition](https://en.wikipedia.org/wiki/Function_composition):

    auto add_1 = partial(sum)(1);
    auto add_2 = compose(add_1, add_1);
    auto three = add_2(1);

Lambdas
-------

Writing function objects can be a little verbose. C++ provides lambdas which have a much terser syntax for defining functions. Of course, lambdas can work with all the adaptors in the library, however, if we want to declare a function using lambdas, [`FIT_STATIC_FUNCTION`](function.md) won't work. Instead, [`FIT_STATIC_LAMBDA_FUNCTION`](lambda.md) can be used to the declare the lambda as a function instead, this will initialize the function at compile-time and avoid possible ODR violations:

    FIT_STATIC_LAMBDA_FUNCTION(sum) = [](auto x, auto y)
    {
        return x + y;
    };

Additionally, adaptors can be used, so the pipable version of `sum` can be written like this:

    // Pipable sum
    FIT_STATIC_LAMBDA_FUNCTION(sum) = pipable([](auto x, auto y)
    {
        return x + y;
    });

