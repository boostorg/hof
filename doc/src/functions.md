
always
======

Description
-----------

The `always` function returns a function object that will always return
the value given to it, no matter what parameters are passed to the
function object. The `always_ref` version will return a reference, and it
requires the value based in to be an lvalue.

Synopsis
--------

    template<class T>
    /* unspecified */ always(T value);

    template<class T>
    /* unspecified */ always_ref(T value);

Example
-------

    int ten = 10;
    assert( zen::always(ten)(1,2,3,4,5) == 10 );

identity
========

Description
-----------

The `identity` function is an unary function object that returns whats given to it. 

Synopsis
--------

    struct
    {
        template<class T>
        constexpr T operator()(T&& x) const
        {
            return fit::forward<T>(x);
        }
    } identity;

