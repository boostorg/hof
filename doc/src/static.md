static
======

Description
-----------

The `static_` adaptor is a static function adaptor that allows any
default-constructible function object to be static-initialized. Functions
initialized by `static_` cannot be used in `constexpr` functions. If the
function needs to be statically initialized and called in a `constexpr`
context, then a `constexpr` constructor needs to be used rather than
`static_`.

Synopsis
--------

    template<class F>
    class static_;

Requirements
------------

F must be:

    FunctionObject
    DefaultConstructible

Example
-------

    // In C++ this class can't be static-initialized, because of the non-
    // trivial default constructor.
    struct times_function
    {
        double factor;
        times2_function() : factor(2)
        {}
        template<class T>
        T operator()(T x) const
        {
            return x*factor;
        }
    };

    static_<times_function> times2 = {};

    assert(6 == times2(3));

