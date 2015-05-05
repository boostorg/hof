apply
=====

Description
-----------

The `apply` function calls the function given to it with its arguments.

Synopsis
--------

    template<class F, class... Ts>
    constexpr auto apply(F&& f, Ts&&... xs);

Requirements
------------

F must be:

    FunctionObject

Example
-------

    struct sum_f
    {
        template<class T, class U>
        T operator()(T x, U y) const
        {
            return x+y;
        }
    };
    assert(fit::apply(sum_f(), 1, 2) == 3);

