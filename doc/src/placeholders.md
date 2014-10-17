placeholders
============

Description
-----------

The placeholders provide `std::bind` compatible placeholders, that
additionally provide basic C++ operators that creates bind expressions.
Each bind expression supports `constexpr` function evaluation.

Synopsis
--------

    placeholder<1> _1 = {};
    placeholder<2> _2 = {};
    placeholder<3> _3 = {};
    placeholder<4> _4 = {};
    placeholder<5> _5 = {};
    placeholder<6> _6 = {};
    placeholder<7> _7 = {};
    placeholder<8> _8 = {};
    placeholder<9> _9 = {};


Example
-------

    auto sum = _1 + _2;
    assert(3 == sum(1, 2));


unamed placeholder
==================

Description
-----------

The unamed placeholder can be used to build simple functions from C++
operators. 

Note: The function produced by the unamed placeholder is not a bind expression.

Synopsis
--------

    /* unspecified */ _ = {};

Example
-------

    auto sum = _ + _;
    assert(3 == sum(1, 2));

