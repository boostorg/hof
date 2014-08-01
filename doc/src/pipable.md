pipable
=======

Description
-----------

The `pipable` function adaptor provides an extension method. The first
parameter of the function can be piped into the function using the pipe
`|` operator. This can be especially convient when there are a lot of
nested function calls. Functions that are made pipable can still be called
the traditional way without piping in the first parameter.

Synopsis
--------

    template<class F>
    pipable_adaptor<F> pipable(F f);

Example
-------

    struct sum
    {
        template<class T>
        T sum(T x, T y) const
        {
            return x+y;
        }
    };

    assert(3 == 1 | pipable(sum())(2));
    assert(3 == pipable(sum())(1, 2));

