mutable
=======

Description
-----------

The `mutable` function adaptor allows using a non-const function object
inside of a const-function object. In Fit, all the function adaptors use
`const` call overloads, so if there is a function that has a non-const
call operator, it couldn't be used directly. So, `mutable_` allows the
function to be used inside of the call operator.

NOTE: This function should be used with caution since many functions are
copied, so relying on some internal shared state can be error-prone.

Synopsis
--------

    template<class F>
    mutable_adaptor<F> mutable_(F f)

Requirements
------------

F must be:

    MutableFunctionObject
    MoveConstructible

