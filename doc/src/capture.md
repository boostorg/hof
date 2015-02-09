capture
=======

Description
-----------

The `capture` function is used to capture values in a function. It
provides more flexibility in capturing than the lambda capture list in
C++. It provides a way to do move and perfect capturing. The values
captured are prepended to the argument list of the function that will be
called.

Synopsis
--------

    // Capture lvalues and rvalues
    template<class... Ts>
    constexpr auto capture(Ts&&... xs);

    // Capture using perfect forwarding
    template<class... Ts>
    constexpr auto capture_forward(Ts&&... xs);

    // Capture by decaying each value
    template<class... Ts>
    constexpr auto capture_decay(Ts&&... xs);


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
    auto add_one = fit::capture(1)(sum_f());
    assert(add_one(2) == 3);

