Requirements
============

This requires a C++11 compiler. There are no third-party dependencies. This has been tested on clang 3.4-3.6, gcc 4.6-5.1, and Visual Studio 2015. 

Partial constexpr
-----------------

All supported compilers support constexpr-initialization of function. However, a few compilers have partial support for constexpr evaluation of functions due to buggy implementation of constexpr:

* GCC 4.6 
* GCC 5.1 
* Visual Studio 2015 
