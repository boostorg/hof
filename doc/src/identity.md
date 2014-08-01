identity
========

Description
-----------

The `identity` function is an unary function object that returns whats given to it. 

Synopsis
--------

    struct
    {
        template<class T>
        constexpr T operator()(T&& x) const
        {
            return fit::forward<T>(x);
        }
    } identity;

