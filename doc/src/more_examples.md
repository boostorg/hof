More examples
=============

As the Fit library is a collection of generic utilities
related to functions, there is many useful cases with the library, but a key
point of many of these utilities is that they can solve these problems with
much simpler constructs than whats traditionally been done with
metaprogramming. Lets take look at some of the use cases for using the Fit library.

Initialization
--------------

The [`FIT_STATIC_FUNCTION`](function.md) will help initialize function objects at
global scope, and will ensure that it is initialized at compile-time and (on
platforms that support it) will have a unique address across translation
units, thereby reducing executable bloat and potential ODR violations.

In addition, [`FIT_STATIC_LAMBDA_FUNCTION`](lambda.md) allows initializing a lambda
in the same manner. This allows for simple and compact function definitions
when working with generic lambdas and function adaptors.

Of course, the library can still be used without requiring global function
objects for those who prefer to avoid them will still find the library useful.

Conditional overloading
-----------------------

Take a look at this example of defining a `stringify` function from
stackoverflow [here](http://stackoverflow.com/questions/30189926/metaprograming-failure-of-function-definition-defines-a-separate-function/30515874).

The user would like to write `stringify` to call `to_string` where applicable
and fallback on using `sstream` to convert to a string. Most of the top
answers usually involve some amount of metaprogramming using either `void_t`
or `is_detected`(see [n4502](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4502.pdf)). However, with the Fit library it can simply be written like
this:

    FIT_STATIC_LAMBDA_FUNCTION(stringify) = conditional(
        [](auto x) FIT_RETURNS(to_string(x)),
        [](auto x) FIT_RETURNS(static_cast<ostringstream&>(ostringstream() << x).str())
    );

In addition, this can be used with the [`fit::if_`](if.md) decorator to create `static_if`-like
constructs. For example, Baptiste Wicht discusses how one could write `static_if` in C++ [here](http://baptiste-wicht.com/posts/2015/07/simulate-static_if-with-c11c14.html).

He wants to be able to write this:

    template<typename T>
    void decrement_kindof(T& value){
        static_if(std::is_same<std::string, T>::value){
            value.pop_back();
        } else {
            --value;
        }
    }

However, that isn't possible in C++. With the Fit library one can simply write
this:

    template<typename T>
    void decrement_kindof(T& value)
    {
        eval(conditional(
            if_(std::is_same<std::string, T>())([&](auto id){
                id(value).pop_back();
            }),
            [&](auto id){
                --id(value);
            }
        ));
    }

The advantage of using the Fit library instead of the solution in Baptiste
Wicht's blog, is that [`conditional`](conditional.md) allows more than just two conditions. So if
there was another trait to be checked, such as `is_stack`, it could be written
like this:

    template<typename T>
    void decrement_kindof(T& value)
    {
        eval(conditional(
            if_(is_stack<T>())([&](auto id){
                id(value).pop();
            }),
            if_(std::is_same<std::string, T>())([&](auto id){
                id(value).pop_back();
            }),
            [&](auto id){
                --id(value);
            }
        ));
    }

Furthermore, this technique can be used to write type traits as well. Jens
Weller was looking for a way to define a general purpose detection for pointer
operands(such as `*` and `->`). One way to accomplish this is using Fit like
this:

    template<class T>
    auto has_pointer_member(const T&) -> decltype(
        &T::operator*,
        &T::operator->,
        std::true_type{}
    );

    FIT_STATIC_LAMBDA_FUNCTION(has_pointer_operators) = conditional(
        FIT_LIFT(has_pointer_member),
        [](auto* x) -> bool_constant<(!std::is_void<decltype(*x)>())> { return {}; },
        always(std::false_type{})
    );

    template<class T>
    struct is_dereferenceable
    : decltype(has_pointer_operators(std::declval<T>()))
    {};

Which is much simpler than the other implementations that were written, which were
about 3 times the amount of code(see [here](https://gist.github.com/lefticus/6fdccb18084a1a3410d5)).

Projections
-----------

Instead of writing the projection multiple times in algorithms:

    std::sort(std::begin(people), std::end(people),
              [](const Person& a, const Person& b) {
                return a.year_of_birth < b.year_of_birth;
              });

We can use the [`by`](by.md) adaptor to project `year_of_birth` on the comparison
operator:

    std::sort(std::begin(people), std::end(people),
            by(&Person::year_of_birth, _ < _));

Ordering evaluation of arguments
--------------------------------

When we write `f(foo(), bar())` there is no guarantee from the standard what
order the arguments `foo()` and `bar()` are evaluated. So with `apply_eval` we
can order them from left-to-right:

apply_eval(f, [&]{ return foo(); }, [&]{ return bar(); });

Varidiac parameters
-------------------

As shown in the [quick start guide](quickstart.md) the [`by`](by.md) adaptor can be used to apply a function to each
argument, so we could write a simple varidiac print function like this:

    FIT_STATIC_FUNCTION(print) = by(std::cout << _);

We can also take binary functions and turn them easily into varidiac functions
as well using [`compress`](compress.md). So a varidiac `max` function could be written like
this:

    FIT_STATIC_FUNCTION(max) = compress(FIT_LIFT(std::max));

Polymorphic constructors
------------------------

Writing polymorphic constructors(such as `make_tuple`) is a boilerplate that
has to be written over and over again for template classes. With [`construct`](construct.md)
this is easier. For example, `make_tuple` can be written simply as this:

    FIT_STATIC_FUNCTION(make_tuple) = construct<std::tuple>().by(decay());

Extension methods
-----------------

Chaining many function together, like what is done for range based libraries
can make things hard to read:

    auto r = transform(
        filter(
            numbers,
            [](int x) { return x > 2; }
        ),
        [](int x) { return x * x; }
    );

It would be nice to write this:

    auto r = numbers
        .filter([](int x) { return x > 2; })
        .transform([](int x) { return x * x; });

However, UFCS in C++17 won't allow this to be done generically. So instead
pipable functions can be used. So it can be written like this:

    auto r = numbers
        | filter([](int x) { return x > 2; })
        | transform([](int x) { return x * x; });

Now, if some users feel a little worried about overloading the bit or
operator, pipable functions can also be used with [`flow`](flow.md) like this:

    auto r = flow(
        filter([](int x) { return x > 2; }),
        transform([](int x) { return x * x; })
    )(numbers);

No fancy or confusing operating overloading and everything is still quite
readable.

