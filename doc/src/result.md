result
======

Description
-----------

The `result` function adaptor sets the return type for the function.

Synopsis
--------

    template<class Result, class F>
    constexpr result_adaptor<Result, F> result(F f);

Requirements
------------

F must be:

    FunctionObject
    MoveConstructible

Example
-------

    struct id
    {
        template<class T>
        T operator()(T x) const
        {
            return x;
        }
    };

    auto int_result = fit::result<int>(id());
    static_assert(std::is_same<decltype(int_result(true)), int>::value, "Not the same type");

