protect
=======

Description
-----------

The `protect` function adaptor can be used to make a bind expression be
treated as a normal function instead.

Synopsis
--------

    template<class F>
    constexpr protect_adaptor<F> protect(F f);

Requirements
------------

F must be:

    FunctionObject
    MoveConstructible

