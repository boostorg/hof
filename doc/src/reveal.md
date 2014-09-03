reveal
======

Description
-----------

The `reveal` function adaptor turns substitution failures into compile errors.
Sometimes an error in a function that causes a substitution failure, will
remove the function from valid overloads thus masking the error inside the
function. The `reveal` adaptor reveals these errors by forcing a compile
error, instead of a substitution failure.

Synopsis
--------

    template<class F>
    reveal_adaptor<F> reveal(F f);

Requirements
------------

Fs must be:

    FunctionObject
    MoveConstructible

