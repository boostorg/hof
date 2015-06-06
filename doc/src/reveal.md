reveal
======

Description
-----------

The `reveal` function adaptor helps shows the error messages that get
masked on some compilers. Sometimes an error in a function that causes a
substitution failure, will remove the function from valid overloads. On
compilers without a backtrace for substitution failure, this will mask the
error inside the function. The `reveal` adaptor will expose these error
messages while still keeping the function SFINAE-friendly.

Synopsis
--------

    template<class F>
    reveal_adaptor<F> reveal(F f);

Requirements
------------

Fs must be:

    FunctionObject
    MoveConstructible

