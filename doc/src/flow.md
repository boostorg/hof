flow
====

Description
-----------

The `flow` function adaptor provides function composition. It is useful as
an alternative to using the pipe operator `|` when chaining functions. It
is similiar to `compose` except the evauluation order is reversed. So,
`flow(f, g)(0)` is equivalent to `g(f(0))`.


Synopsis
--------

    template<class... Fs>
    constexpr flow_adaptor<Fs...> flow(Fs... fs);

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

    int r = flow(increment(), decrement(), increment())(3);
    assert(r == 4);

