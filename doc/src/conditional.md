conditional
===========

Description
-----------

The `conditional` function adaptor combines several functions together. If
the first function can not be called, then it will try to call the next
function. This can be very useful when overloading functions using
template constraints(such as with `enable_if`).

Note: This is different than the `match` function adaptor, which can lead
to ambiguities. Instead, `conditional` will call the first function that
is callable, regardless if there is another function that could be called
as well. 

Synopsis
--------

    template<class... Fs>
    constexpr conditional_adaptor<Fs...> conditional(Fs... fs);

Requirements
------------

Fs must be:

    FunctionObject
    MoveConstructible

Example
-------

    struct for_ints
    {
        void operator()(int) const
        {
            printf("Int\n");
        }
    };

    struct for_floats
    {
        void operator()(int) const
        {
            printf("Float\n");
        }
    };

    conditional(for_ints(), for_floats())(3.0);

This will print `Int` because the `for_floats` function object won't ever be
called. Due to the conversion rules in C++, the `for_ints` function can be
called on floats, so it is chosen by `conditional` first, even though
`for_floats` is a better match.

So, the order of the functions in the `conditional_adaptor` are very important
to how the function is chosen.
