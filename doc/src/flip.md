flip
====

Description
-----------

The `flip` function adaptor swaps the first two parameters.

Synopsis
--------

    template<class F>
    flip_adaptor<F> flip(F f);

Requirements
------------

F must be:

    BinaryFunctionObject
    MoveConstructible

Example
-------

    int r = fit::flip(fit::_ - fit::_)(2, 5);
    assert(r == 3);

