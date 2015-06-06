pipable
=======

Description
-----------

The `pipable` function adaptor provides an extension method. The first
parameter of the function can be piped into the function using the pipe
`|` operator. This can be especially convenient when there are a lot of
nested function calls. Functions that are made pipable can still be called
the traditional way without piping in the first parameter.

Synopsis
--------

    template<class F>
    constexpr pipable_adaptor<F> pipable(F f);

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

    assert(3 == 1 | pipable(sum())(2));
    assert(3 == pipable(sum())(1, 2));

