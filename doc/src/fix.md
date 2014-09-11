fix
===

Description
-----------

The `fix` function adaptor implements a fixed-point combinator. This can be
used to write recursive functions. 

Note: Compilers are too eager to instantiate templates when using
constexpr, which causes the compiler to reach its internal instantiation
limit. So, unfortunately, `fix` cannot be used for `constexpr` functions.

Synopsis
--------

    template<class F>
    constexpr fix_adaptor<F> fix(F f);

Requirements
------------

F must be:

    FunctionObject
    MoveConstructible

Example
-------

    int r = fit::fix([](auto s, auto x) -> decltype(x) { return x == 0 ? 1 : x * s(x-1); })(5);
    assert(r == 5*4*3*2*1);

