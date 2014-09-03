on
==

Description
-----------

The `on` function adaptor applies a projection onto the parameters of
another function. This is useful, for example, to define a function for
sorting such that the ordering is based off of the value of one of its
member fields.

Synopsis
--------

    template<class Projection, class F>
    constexpr on_adaptor<Projection, F> on(Projection p, F f);

Requirements
------------

Projection must be:

    UnaryFunctionObject
    MoveConstructible

F must be:

    FunctionObject
    MoveConstructible

Example
-------

    struct foo
    {
        foo(int x) : x(x)
        {}
        int x;
    };
    assert(fit::on(std::mem_fn(&foo::x), _ + _)(foo(1), foo(2)) == 3);

