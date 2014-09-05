indirect
========

Description
-----------

The `indirect` function adaptor dereferences the object before calling it.

Synopsis
--------

    template<class F>
    constexpr indirect_adaptor<F> indirect(F f);

Requirements
------------

F must be:

    FunctionObject
    MoveConstructible
    Dereferenceable

Example
-------

    struct sum
    {
        template<class T>
        T sum(T x, T y)
        {
            return x+y;
        }
    };

    int r = indirect(std::make_unique<sum>())(3,2);
    assert(r, 5);

