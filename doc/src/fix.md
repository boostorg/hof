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

