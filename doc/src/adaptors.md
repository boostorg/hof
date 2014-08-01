
compose
======

Description
-----------

The `compose` function adaptor provides funcion composition. It produces a
function object that composes a set of functions, ie the output of one
function becomes the input of the second function. So, `compose(f, g)(0)` is
equivalent to `f(g(0))`.


Synopsis
--------

    template<class F1, class F2, ...>
    compose_adaptor<F1, F2, ...> compose(F1 f1, F2 f2, ...);

Example
-------

    struct increment
    {
        template<class T>
        T operator()(T x) const
        {
            return x + 1;
        }
    };

    struct decrement
    {
        template<class T>
        T operator()(T x) const
        {
            return x - 1;
        }
    };

    int r = compose(increment(), decrement(), increment())(3);
    assert(r == 4);

conditional
===========

Description
-----------

The `conditional` function adaptor combines several functions together. If the
first function can not be called, then it will try to call the next function.

Note: This is different than the `match` function adaptor, which can lead
to ambiguities. Instead, `conditional` will call the first function that
is callable, regardless if there is another function that could be called
as well. So, for example:

    struct for_ints
    {
        void operator()(int) const
        {
            printf("Int\n");
        }
    };

    struct for_floats
    {
        void operator()(int) const
        {
            printf("Float\n");
        }
    };

    conditional(for_ints(), for_floats())(3.0);

This will print `Int` because the `for_floats` function object won't ever be
called. Due to the conversion rules in C++, the `for_ints` function can be
called on floats, so it is chosen by `conditional` first, even though
`for_floats` is a better match.

So, the order of the functions in the `conditional_adaptor` are very important
to how the function is chosen.

Synopsis
--------

    template<class F1, class F2, ...>
    conditional_adaptor<F1, F2, ...> conditional(F1 f1, F2 f2, ...);

fix
===

Description
-----------

The `fix` function adaptor implements a fixed-point combinator. This can be
used to write recursive functions.

Synopsis
--------

    template<class F>
    fix_adaptor<F> fix(F f);

Example
-------

    int r = fit::fix([](auto s, auto x) -> decltype(x) { return x == 0 ? 1 : x * s(x-1); })(5);
    assert(r, 5*4*3*2*1);

fuse
====

Description
-----------

The `fuse` function adaptor takes a tuple and uses that for the arguments
to the function.

Synopsis
--------

    template<class F>
    fuse_adaptor<F> fuse(F f);

Example
-------

    struct sum
    {
        template<class T>
        T sum(T x, T y)
        {
            return x+y;
        }
    };

    int r = fuse(sum())(std::make_tuple(3,2));
    assert(r, 5);

implicit
========

Description
-----------

The `implicit` adaptor is a static function adaptor that uses the type that
return value can be converted to in order to determine the type of the
template parameter. Since it is static function adaptor, the function must be
default constructible.

Synopsis
--------

    template<template <class...> class F>
    class implicit<F>;

Example
-------

    template<class T>
    struct auto_caster
    {
        template<class U>
        T operator()(U x)
        {
            return T(x);
        }
    };

    implicit<auto_caster> auto_cast = {};

    struct auto_caster_foo
    {
        int i;
        explicit auto_caster_foo(int i) : i(i) {}

    };

    float f = 1.5;
    int i = auto_cast(f);
    auto_caster_foo x = auto_cast(1);
    assert(1 == i);
    assert(1 == x.i);

invoke
======

Description
-----------

Calls a function object with the arguments from a tuple.

Synopsis
--------

    template<class F, class Sequence>
    auto invoke(F f, const Sequence& seq);

Example
-------

    std::plus<int> add;
    assert(invoke(add,std::make_tuple(1,1)) == 2);

lazy
====

Description
-----------

The `lazy` function adaptor returns a function object call wrapper for a
function. Calling this wrapper is equivalent to invoking the function. It
is a simple form of lambda expressions, but is constexpr friendly.

Ultimately, calling `lazy(f)(x)` is the equivalent to calling
`std::bind(f, x)` except the lazy version can be called in a constexpr
context, as well. The `lazy` adaptor is compatible with `std::bind`, so
most of the time `lazy` and `std::bind` can be used interchangeably.
However, the `lazy` adaptor won't accept member function pointers, like
`std::bind` will.

Synopsis
--------

    template<class F>
    lazy_adaptor<F> lazy(F f);

Example
-------

    auto add = [](auto x, auto y) { return x+y; }
    auto increment = lazy(add)(_1, 1);
    assert(increment(5) == 6);

match
=====

Description
-----------

The `match` function adaptor combines several functions together and
resolves which one should be called by using C++ overload resolution. This
is different than than the `conditional` adaptor which resolves them based
on order.

Synopsis
--------

    template<class F1, class F2, ...>
    match_adaptor<F1, F2, ...> match(F1 f1, F2 f2, ...);

Example
-------

    struct int_class
    {
        int operator()(int) const
        {
            return 1;
        }
    };

    struct foo
    {};

    struct foo_class
    {
        foo operator()(foo) const
        {
            return foo();
        }
    };

    typedef match_adaptor<int_class, foo_class> fun;

    static_assert(std::is_same<int, decltype(fun()(1))>::value, "Failed match");
    static_assert(std::is_same<foo, decltype(fun()(foo()))>::value, "Failed match");

mutable
=======

Description
-----------

The `mutable` function adaptor allows using a non-const function object
inside of a const-function object. In Fit, all the function adaptors use
`const` call overloads, so if there is a function that has the a non-const
call operator, it couldn't be used directly. So, `mutable_` allows the
function to be used inside of the call operator. NOTE: This function
should be used with caution since many functions are copied, so relying on
some internal shared state can be error-prone.

Synopsis
--------

    template<class F>
    mutable_adaptor<F> mutable_(F f)

on
==

Description
-----------

The `on` function adaptor applies a projection onto the parameters to
another function. This is useful for defining function such for ordering
such that the ordering is based on the one member function.

Synopsis
--------

    template<class Projection, class F>
    on_adaptor<Projection, F> on(Projection p, F f);

Example
-------

    struct foo
    {
        foo(int x) : x(x)
        {}
        int x;
    };
    assert(fit::on(std::mem_fn(&foo::x), _ + _)(foo(1), foo(2)) == 3);

partial
========

Description
-----------

The `partial` function adaptor allows partial application of the function.
If the function can not be called with all the parameters, it will return a
function. It will continually do this until the function can be called. By
default, the `partial` captures all of it variables by value, just like
bind. `std::ref` can be used to capture references instead.

Synopsis
--------

    template<class F>
    partial_adaptor<F> partial(F f);

Example
-------

    struct sum
    {
        template<class T>
        T sum(T x, T y)
        {
            return x+y;
        }
    };

    assert(3 == partial(sum())(1)(2));

pipable
=======

Description
-----------

The `pipable` function adaptor provides an extension method. The first
parameter of the function can be piped into the function using the `|`
operator. This can be especially convient when there are a lot of nested
function calls. Functions that are made pipable can still be called the
traditional way without piping in the first parameter.

Synopsis
--------

    template<class F>
    pipable_adaptor<F> pipable(F f);

Example
-------

    struct sum
    {
        template<class T>
        T sum(T x, T y) const
        {
            return x+y;
        }
    };

    assert(3 == 1 | pipable(sum())(2));
    assert(3 == pipable(sum())(1, 2));

reveal
======

Description
-----------

The `reveal` function adaptor turns substitution failures into compile errors.
Sometimes an error in a function that causes a substitution failure, will
remove the function from valid overloads thus masking the error inside the
function. The `reveal` adaptor reveals these errors by forcing a compile
error, instead of a substitution failure.

Synopsis
--------

    template<class F>
    reveal_adaptor<F> reveal(F f);

static
======

Description
-----------

The `static_` adaptor is a static function adaptor that allows any
default-constructible function object to be static-initialized. Functions
initialized by `static_` cannot be used in `constexpr` functions. If the
function needs to be statically initialized and called in a `constexpr`
context, then a `constexpr` constructor needs to be used rather than
`static_`.

Synopsis
--------

    template<class F>
    class static_;

Example
-------

    // In C++ this class can't be static-initialized, because of the non-
    // trivial default constructor.
    struct times_function
    {
        double factor;
        times2_function() : factor(2)
        {}
        template<class T>
        T operator()(T x) const
        {
            return x*factor;
        }
    };

    static_<times_function> times2 = {};

    assert(6 == times2(3));

variadic
========

Description
-----------

The `variadic` function adaptor converts the arguments to the function to a
tuple.

Synopsis
--------

    template<class F>
    variadic_adaptor<F> variadic(F f);

