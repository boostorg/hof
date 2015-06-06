pack
====

Description
-----------

The `pack` function returns a higher order function object that takes a
function that will be passed the initial elements. The function object is
a sequence that can be unpacked with `unpack_adaptor` as well. Also,
`pack_join` can be used to join multiple packs together.

Synopsis
--------

    // Capture lvalues by reference and rvalues by value.
    template<class... Ts>
    constexpr auto pack(Ts&&... xs);

    // Capture lvalues by reference and rvalue reference by reference
    template<class... Ts>
    constexpr auto pack_perfect(Ts&&... xs);

    // Decay everything before capturing
    template<class... Ts>
    constexpr auto pack_decay(Ts&&... xs);

    // Join multiple packs together
    template<class... Ts>
    constexpr auto pack_join(Ts&&... xs);


Example
-------

    struct sum
    {
        template<class T, class U>
        T operator()(T x, U y) const
        {
            return x+y;
        }
    };

    int r = pack(3, 2)(sum());
    assert(r == 5);

