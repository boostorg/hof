args
====

Description
-----------

The `args` function returns the Nth argument passed to it. It actually
starts at 1, so it is not the zero-based index of the argument.

Synopsis
--------

    template<int N, class... Ts>
    constexpr auto args(Ts&&... xs);

Example
-------

    assert(args<3>(1,2,3,4,5) == 3);

