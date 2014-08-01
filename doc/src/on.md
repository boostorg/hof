on
==

Description
-----------

The `on` function adaptor applies a projection onto the parameters to
another function. This is useful for defining function such for ordering
such that the ordering is based on the one member function.

Synopsis
--------

    template<class Projection, class F>
    on_adaptor<Projection, F> on(Projection p, F f);

Example
-------

    struct foo
    {
        foo(int x) : x(x)
        {}
        int x;
    };
    assert(fit::on(std::mem_fn(&foo::x), _ + _)(foo(1), foo(2)) == 3);

