partial
========

Description
-----------

The `partial` function adaptor allows partial application of the function.
If the function can not be called with all the parameters, it will return
another function. It will continually do this until the function can
finally be called. By default, `partial` captures all of its variables by
value, just like bind. `std::ref` can be used to capture references
instead.

Synopsis
--------

    template<class F>
    constexpr partial_adaptor<F> partial(F f);

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

    assert(3 == partial(sum())(1)(2));

