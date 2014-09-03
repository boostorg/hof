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
    constexpr lazy_adaptor<F> lazy(F f);

Requirements
------------

F must be:

    FunctionObject
    MoveConstructible

Example
-------

    auto add = [](auto x, auto y) { return x+y; }
    auto increment = lazy(add)(_1, 1);
    assert(increment(5) == 6);

