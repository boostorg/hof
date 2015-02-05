returns
=======

Description
-----------

The `FIT_RETURNS` macro helps deduce the return type of an expression
using the trailing return type. Instead of repeating the expression for
the return type and using the expression in the function body, this macro
helps reduce the code duplication from that.

Note: The expression used to deduce the return the type will also
constrain the template function as well, which is different behaviour than
using C++14's return type deduction.

Synopsis
--------

    #define FIT_RETURNS(...) 


Example
-------

    template<class T, class U>
    auto sum(T x, U y) FIT_RETURNS(x+y);

    assert(3 == sum(1, 2));


Incomplete this
===============

Description
-----------

On older compilers the `this` variable cannot be used inside the
`FIT_RETURNS` macro because it is considered an incomplete type. So the
following macros are provided to help workaround the issue.


Synopsis
--------

    // Declares the type of the `this` variable
    #define FIT_RETURNS_CLASS(...) 
    // Used to refer to the `this` variable in the FIT_RETURNS macro
    #define FIT_THIS
    // Used to refer to the const `this` variable in the FIT_RETURNS macro
    #define FIT_CONST_THIS


Example
-------

    struct add_1
    {
        int a;
        add_1() : a(1) {}
        
        FIT_RETURNS_CLASS(add_1)
        
        template<class T>
        auto operator()(T x) const 
        FIT_RETURNS(x+FIT_CONST_THIS->a);
    };

    assert(3 == add_1()(2));


Mangling overloads
==================

Description
-----------

On older compilers some operations done in the exressions cannot be
properly mangled. These macros help provide workarounds for these
operations used on these older compilers.


Synopsis
--------

    // Explicitly defines the type for name mangling
    #define FIT_MANGLE_CAST(...) 
    // C cast for name mangling
    #define FIT_RETURNS_C_CAST(...) 
    // Reinterpret cast for name mangling
    #define FIT_RETURNS_REINTERPRET_CAST(...) 
    // Static cast for name mangling
    #define FIT_RETURNS_STATIC_CAST(...) 
    // Construction for name mangling
    #define FIT_RETURNS_CONSTRUCT(...) 

