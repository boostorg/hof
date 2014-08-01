
args
====

Description
-----------

The `args` function returns the Nth argument passed to it. It actually
starts at 1, it is not the zero-based index of the argument.

Synopsis
--------

    template<int N, class... Ts>
    constexpr auto args(Ts&&... xs);

Example
-------

    assert(args<3>(1,2,3,4,5) == 3);

is_callable
===========

Description
-----------

The `is_callable` metafunction checks if the function object is callable with
the certain parameters. It uses the same signature as `result_of`.


Synopsis
--------

    template<class Sig>
    struct is_callable;

Example
-------

    struct is_callable_class
    {
        void operator()(int) const
        {
        }
    };
    static_assert(is_callable<is_callable_class(int)>(), "Not callable");

