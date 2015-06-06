implicit
========

Description
-----------

The `implicit` adaptor is a static function adaptor that uses the type
that the return value can be converted to in order to determine the type
of the template parameter. In essence, it will deduce the type for the
template parameter using the type of variable the result is assigned to.
Since it is a static function adaptor, the function must be default
constructible.

Synopsis
--------

    template<template <class...> class F>
    class implicit<F>;

Requirements
------------

F must be a template class, that is a:

    FunctionObject
    DefaultConstructible

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

