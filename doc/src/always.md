always
======

Description
-----------

The `always` function returns a function object that will always return
the value given to it, no matter what parameters are passed to the
function object. The `always_ref` version will return a reference, and it
requires the value passed in to be an lvalue.

Synopsis
--------

    template<class T>
    constexpr /* unspecified */ always(T value);

    template<class T>
    constexpr /* unspecified */ always_ref(T& value);

Requirements
------------

T must be:

    CopyConstructible

Example
-------

    int ten = 10;
    assert( always(ten)(1,2,3,4,5) == 10 );

