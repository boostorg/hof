eval
====

Description
-----------

The `eval` function will evaluate a "thunk". This can be either a nullary
function or it can be a an unary function that takes the identity function
as the first parameter(which is helpful to delay compile-time checking).
Also, additional parameters can be passed to `eval` to delay compiliation(so
that result can depend on template parameters).

Synopsis
--------

    template<class F, class... Ts>
    constexpr auto eval(F&& f, Ts&&...);

Requirements
------------

F must be:

    EvaluatableFunctionObject

Example
-------

    assert(fit::eval([]{ return 3; }) == 3);

