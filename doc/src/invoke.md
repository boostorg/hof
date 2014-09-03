invoke
======

Description
-----------

Calls a function object with the arguments from a tuple.

Synopsis
--------

    template<class F, class Sequence>
    auto invoke(F f, const Sequence& seq);

Requirements
------------

F must be:

    FunctionObject
    MoveConstructible

Sequence must be a:

    TupleSequence

Example
-------

    std::plus<int> add;
    assert(invoke(add,std::make_tuple(1,1)) == 2);

