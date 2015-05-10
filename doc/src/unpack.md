unpack
======

Description
-----------

The `unpack` function adaptor takes a sequence and uses the elements of
the sequence for the arguments to the function. Multiple sequences can be
passed to the function. All elements from each sequence will be passed
into the function. 


Synopsis
--------

    template<class F>
    unpack_adaptor<F> unpack(F f);

Requirements
------------

F must be:

    FunctionObject
    MoveConstructible

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

    int r = unpack(sum())(std::make_tuple(3,2));
    assert(r == 5);


is_unpackable
=============

This is a trait that can be used to detect whethet the type can be called
with `unpack`.

Synopsis
--------

    template<class T>
    struct is_unpackable;

Example
-------

    static_assert(fit::is_unpackable<std::tuple<int>>::value, "Failed");

unpack_sequence
===============

How to unpack a sequence can be defined by specializing `unpack_sequence`.
By default, `std::tuple` can be used with unpack.

Synopsis
--------

    template<class Sequence, class=void>
    struct unpack_sequence;

Example
-------

    template<class... Ts>
    struct unpack_sequence<my_sequence<Ts...>>
    {
        template<class F, class Sequence>
        constexpr static auto apply(F&& f, Sequence&& s) FIT_RETURNS
        (
            s(std::forward<F>(f))
        );
    };

