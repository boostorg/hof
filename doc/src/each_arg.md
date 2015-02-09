each_arg
========

Description
-----------

The `each_arg` function will apply the function to each argument that is
passed in.

Synopsis
--------

    template <class F, class... Ts> 
    constexpr void each_arg(F&& f, Ts&&... a);

Requirements
------------

F must be:

    FunctionObject

Example
-------

    std::string s;
    auto f = [&](const std::string& x)
    {
        s += x;
    };
    fit::each_arg(f, "hello", "-", "world");
    assert(s == "hello-world");

