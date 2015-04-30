args
====

Description
-----------

The `args` returns a function object that returns the Nth argument passed
to it. It actually starts at 1, so it is not the zero-based index of the
argument.

Synopsis
--------

    template<class IntegralConstant>
    constexpr auto args(IntegralConstant);


Example
-------

    assert(args(std::integral_constant<int, 3>())(1,2,3,4,5) == 3);

