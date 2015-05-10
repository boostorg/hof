apply_eval
==========

Description
-----------

The `apply_eval` function work like apply, except it calls `eval` on each of
its arguments. Each `eval` call is always ordered from left-to-right.

Synopsis
--------

    template<class F, class... Ts>
    constexpr auto apply_eval(F&& f, Ts&&... xs);

Requirements
------------

F must be:

    FunctionObject

Ts must be:

    Evaluatable

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
    assert(fit::apply_eval(sum_f(), []{ return 1; }, []{ return 2; }) == 3);

