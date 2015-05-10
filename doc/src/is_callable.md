is_callable
===========

Description
-----------

The `is_callable` metafunction checks if the function object is callable with
the certain parameters.


Synopsis
--------

    template<class F, class... Ts>
    struct is_callable;

Example
-------

    struct is_callable_class
    {
        void operator()(int) const
        {
        }
    };
    static_assert(is_callable<is_callable_class, int>(), "Not callable");

