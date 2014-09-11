infix
=====

Description
-----------

The `infix` function adaptor allows the function to be used as an infix
operator. The operator must be placed inside the angle brackets(ie `<`
and `>`).

Synopsis
--------

    template<class F>
    constexpr infix_adaptor<F> infix(F f);

Requirements
------------

F must be:

    BinaryFunctionObject
    MoveConstructible

Example
-------

    struct plus_f
    {
        template<class T, class U>
        T operator()(T x, U y) const
        {
            return x+y;
        }
    };
    
    constexpr infix_adaptor<plus_f> plus = {};
    int r = 3 <plus> 2;
    assert(r, 5);

