variadic
========

Description
-----------

The `variadic` function adaptor converts the arguments of the function to a
tuple.

Synopsis
--------

    template<class F>
    constexpr variadic_adaptor<F> variadic(F f);

Requirements
------------

F must be:

    FunctionObject
    MoveConstructible

