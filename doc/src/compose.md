compose
======

Description
-----------

The `compose` function adaptor provides function composition. It produces
a function object that composes a set of functions, ie the output of one
function becomes the input of the second function. So, `compose(f, g)(0)`
is equivalent to `f(g(0))`.


Synopsis
--------

    template<class... Fs>
    constexpr compose_adaptor<Fs...> compose(Fs... fs);

Requirements
------------

Fs must be:

    FunctionObject
    MoveConstructible

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

