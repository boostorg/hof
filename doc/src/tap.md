tap
===

Description
-----------

The `tap` function invokes a function on the first argument passed in and
then returns the first argument. This is useful in a chain of pipable
function to perform operations on intermediate results. As a result, this
function is pipable.

Synopsis
--------

    template<class T, class F>
    pipable constexpr T tap(T&& x, const F& f);

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

    const pipable<sum_f> sum = {};
    // Prints 3
    int r = 1 | sum(2) | tap([](int i) { std::cout << i; }) | sum(2);
    assert(r == 5);

