match
=====

Description
-----------

The `match` function adaptor combines several functions together and
resolves which one should be called by using C++ overload resolution. This
is different than than the `conditional` adaptor which resolves them based
on order.

Synopsis
--------

    template<class F1, class F2, ...>
    match_adaptor<F1, F2, ...> match(F1 f1, F2 f2, ...);

Example
-------

    struct int_class
    {
        int operator()(int) const
        {
            return 1;
        }
    };

    struct foo
    {};

    struct foo_class
    {
        foo operator()(foo) const
        {
            return foo();
        }
    };

    typedef match_adaptor<int_class, foo_class> fun;

    static_assert(std::is_same<int, decltype(fun()(1))>::value, "Failed match");
    static_assert(std::is_same<foo, decltype(fun()(foo()))>::value, "Failed match");

