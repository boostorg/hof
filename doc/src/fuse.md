fuse
====

Description
-----------

The `fuse` function adaptor takes a tuple and uses that for the arguments
to the function.

Synopsis
--------

    template<class F>
    fuse_adaptor<F> fuse(F f);

Requirements
------------

F must be:

    FunctionObject
    MoveConstructible

Example
-------

    struct sum
    {
        template<class T, class U>
        T operator()(T x, U y) const
        {
            return x+y;
        }
    };

    int r = fuse(sum())(std::make_tuple(3,2));
    assert(r, 5);

