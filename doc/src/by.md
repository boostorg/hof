by
==

Description
-----------

The `by` function adaptor applies a projection onto the parameters of
another function. This is useful, for example, to define a function for
sorting such that the ordering is based off of the value of one of its
member fields. 

Also, if just a projection is given, then the projection will be called
for each of its arguments.

Note: All projections are always evaluated in order from left-to-right.

Synopsis
--------

    template<class Projection, class F>
    constexpr by_adaptor<Projection, F> by(Projection p, F f);

    template<class Projection>
    constexpr by_adaptor<Projection> by(Projection p);

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
    assert(fit::by(std::mem_fn(&foo::x), _ + _)(foo(1), foo(2)) == 3);

