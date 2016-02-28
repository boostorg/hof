Requirements
============

This requires a C++11 compiler. There are no third-party dependencies. This has been tested on clang 3.4-3.7, gcc 4.6-4.9, and Visual Studio 2015. Gcc 5.1 is not supported at all.

Contexpr support
----------------

Both MSVC and gcc 4.6 have limited constexpr support due to many bugs in the implementation of constexpr. However, constexpr initialization of functions is supported when using the `FIT_STATIC_FUNCTION` and `FIT_STATIC_LAMBDA_FUNCTION` constructs.
