FAQ
===

#### Q: Is the reinterpret cast in FIT_STATIC_LAMBDA undefined behaviour?

Not really, since the objects are empty, there is no data access. I have a
static assert to guard against this restriction.

Now there could be an insane implementation where this doesn't work(perhaps
the lambdas are not empty for some strange reason), which the library would
have to apply a different technique to make it work. However, this is quite
unlikely considering that C++ will probably get constexpr lambdas and inline
variables in the future.
