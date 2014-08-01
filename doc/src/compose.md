compose
======

Description
-----------

The `compose` function adaptor provides funcion composition. It produces a
function object that composes a set of functions, ie the output of one
function becomes the input of the second function. So, `compose(f, g)(0)` is
equivalent to `f(g(0))`.


Synopsis
--------

    template<class F1, class F2, ...>
    compose_adaptor<F1, F2, ...> compose(F1 f1, F2 f2, ...);

Example
-------

    struct increment
    {
        template<class T>
        T operator()(T x) const
        {
            return x + 1;
        }
    };

    struct decrement
    {
        template<class T>
        T operator()(T x) const
        {
            return x - 1;
        }
    };

    int r = compose(increment(), decrement(), increment())(3);
    assert(r == 4);

