reverse_compress
========

Description
-----------

The `reverse_compress` function adaptor uses a binary function to apply a
reverse fold(ie right fold in functional programming terms) operation to
the arguments passed to the function. Additionally, an optional initial
state can be provided, otherwise the first argument is used as the initial
state.

The arguments to the binary function, take first the state and then the
argument.

Synopsis
--------

    template<class F, class State>
    constexpr reverse_compress_adaptor<F, State> reverse_compress(F f, State s);

    template<class F>
    constexpr reverse_compress_adaptor<F> reverse_compress(F f);

Requirements
------------

State must be:

    MoveConstructible

F must be:

    BinaryFunctionObject
    MoveConstructible

Example
-------

    struct max_f
    {
        template<class T, class U>
        constexpr T operator()(T x, U y) const
        {
            return x > y ? x : y;
        }
    };
    assert(fit::reverse_compress(max_f())(2, 3, 4, 5) == 5);

