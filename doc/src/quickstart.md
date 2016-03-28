Quick Start
===========

Function Objects
----------------

The Fit library can work with all different types of functions in C++, including static functions, member functions, lambdas, and function objects. Function pointer(or member function pointers) can only refer to one function in an overload set of functions, and it requires explicit casting to select that overload:

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
    int total = std::accumulate(v.begin(), v.end(), 0, sump);


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

Now, this is useful for local functions. However, many times we want to write functions and make them available for others to use. The Fit library provides [`FIT_STATIC_FUNCTION`](function.md) to declare the function object at global or namespace scope:

    FIT_STATIC_FUNCTION(sum) = sum_f();

The `FIT_STATIC_FUNCTION` declares a global variable following the best practices as outlined in [N4381](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4381.html). This includes initializing the function object compile-time to avoid the [static initialization order fiasco](https://isocpp.org/wiki/faq/ctors#static-init-order) and the address of the function object is the same across translation units to avoid possible One-Definition-Rule(ODR) violations.

Adaptors
--------

Now we have defined the function as a function object, we can add new "enhancements" to the function. One enhancement is to write "exentsion" methods. The proposal [N4165](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4165.pdf) for Unified Call Syntax(UFCS) in C++17 would have allowed a function call of `x.f(y)` to become `f(x, y)`. Instead we can use pipable function which would transform `x | f(y)` into `f(x, y)`. We could make the `sum` function pipable using the [`pipable`](pipable.md) adaptor:

    FIT_STATIC_FUNCTION(sum) = pipable(sum_f());

Then the parameters can be piped into it, like this:

    auto three = 1 | sum(2);

Pipable function can be chained mutliple times just like the `.` operator:

    auto four = 1 | sum(2) | sum(1);

Alternatively, instead of using the `|` operator, pipable functions can be chaned together using the [`flow`](flow.md) adaptor:

    auto four = flow(sum(2), sum(1))(1); 

Another enhancement that can be done to functions is defining named infix operators using the [`infix`](infix.md) adaptor:

    FIT_STATIC_FUNCTION(sum) = infix(sum_f());

And it could be called like this:

    auto three = 1 <sum> 2;

In addition, adaptors are provided that support simple functional operations such as partial application and function composition:

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

Overloading
-----------

Now, Fit provides two ways of doing overloading. The [`match`](match.md) adaptor will call a function based on C++ overload resolution, which tries to find the best match, like this:

    FIT_STATIC_LAMBDA_FUNCTION(print) = match(
        [](int x)
        {
            std::cout << "Integer: " << x << std::endl;
        },
        [](const std::string& x)
        {
            std::cout << "String: " << x << std::endl;
        }
    );

However, when trying to do overloading involving something more generic, it can lead to ambiguities. So the [`conditional`](conditional.md) adaptor will pick the first function that is callable. This allows ordering the functions based on which one is more important. Say we would like to write a `print` function that can print not only using `cout` but can also print the values in ranges. We could write something like this:


    FIT_STATIC_LAMBDA_FUNCTION(print) = conditional(
        [](const auto& x) -> decltype(std::cout << x, void())
        {
            std::cout << x << std::endl;
        },
        [](const auto& range)
        {
            for(const auto& x:range) std::cout << x << std::endl;
        }
    );

So the `-> decltype(std::cout << x, void())` will only make the function callable if `std::cout << x` is callable. Then the `void()` is used to return `void` from the function. We can constrain the second overload as well, but we will need some helper function in order to call `std::begin` using ADL lookup:

    namespace adl {

    using std::begin;

    template<class R>
    auto adl_begin(R&& r) FIT_RETURNS(begin(r));
    }

    FIT_STATIC_LAMBDA_FUNCTION(print) = conditional(
        [](const auto& x) -> decltype(std::cout << x, void())
        {
            std::cout << x << std::endl;
        },
        [](const auto& range) -> decltype(std::cout << *adl::adl_begin(range), void())
        {
            for(const auto& x:range) std::cout << x << std::endl;
        }
    );

Tuples
------

We could extend this to printing tuples as well. We will need to combine a couple of functions to make a `for_each_tuple`, which lets us call a function for each element. First, the [`by`](by.md) adaptor will let us apply a function to each argument passed in, and the [`unpack`](unpack.md) adaptor will unpack the elements of a tuple and apply them to the argument:

    FIT_STATIC_LAMBDA_FUNCTION(for_each_tuple) = [](const auto& sequence, auto f)
    {
        return unpack(by(f))(sequence);
    };

So now we can add an overload for tuples:

    FIT_STATIC_LAMBDA_FUNCTION(print) = conditional(
        [](const auto& x) -> decltype(std::cout << x, void())
        {
            std::cout << x << std::endl;
        },
        [](const auto& range) -> decltype(std::cout << *adl::adl_begin(range), void())
        {
            for(const auto& x:range) std::cout << x << std::endl;
        },
        [](const auto& tuple)
        {
            for_each_tuple(tuple, [](const auto& x)
            {
                std::cout << x << std::endl;
            });
        }
    );

Recursive
---------

Even though we are using lambdas, we can easily make this recursive using the [`fix`](fix.md) adaptor. This implements a fix point combinator, which passes the function(i.e. itself) in as the first argument, so we could write this:

    FIT_STATIC_LAMBDA_FUNCTION(print) = fix(conditional(
        [](auto, const auto& x) -> decltype(std::cout << x, void())
        {
            std::cout << x << std::endl;
        },
        [](auto self, const auto& range) -> decltype(self(*adl::adl_begin(range)), void())
        {
            for(const auto& x:range) self(x);
        },
        [](auto self, const auto& tuple) -> decltype(for_each_tuple(tuple, self), void())
        {
            return for_each_tuple(tuple, self);
        }
    ));

Variadic
--------

We can also make this `print` function variadic, so it prints every argument passed into it. We just rename our original `print` function to `simple_print`:

    FIT_STATIC_LAMBDA_FUNCTION(simple_print) = fix(conditional(
        [](auto, const auto& x) -> decltype(std::cout << x, void())
        {
            std::cout << x << std::endl;
        },
        [](auto self, const auto& range) -> decltype(self(*adl::adl_begin(range)), void())
        {
            for(const auto& x:range) self(x);
        },
        [](auto self, const auto& tuple) -> decltype(for_each_tuple(tuple, self), void())
        {
            return for_each_tuple(tuple, self);
        }
    ));

And then apply the [`by`](by.md) adaptor to `simple_print`:

    FIT_STATIC_LAMBDA_FUNCTION(print) = by(simple_print);

