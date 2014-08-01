partial
========

Description
-----------

The `partial` function adaptor allows partial application of the function.
If the function can not be called with all the parameters, it will return a
function. It will continually do this until the function can be called. By
default, the `partial` captures all of it variables by value, just like
bind. `std::ref` can be used to capture references instead.

Synopsis
--------

    template<class F>
    partial_adaptor<F> partial(F f);

Example
-------

    struct sum
    {
        template<class T>
        T sum(T x, T y)
        {
            return x+y;
        }
    };

    assert(3 == partial(sum())(1)(2));

