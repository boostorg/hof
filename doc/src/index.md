
Introduction
============

Fit is a header-only C++11/C++14 library that provides utilities for functions and function objects, which can solve many problems with much simpler constructs than whats traditionally been done with metaprogramming.

Fit is:

- Modern: Fit takes advantages of modern C++11/C++14 features. It support both `constexpr` initialization and `constexpr` evaluation of functions. It takes advantage of type deduction, varidiac templates, and perfect forwarding to provide a simple and modern interface. 
- Relevant: Fit provides utilities for functions and does not try to implement a functional language in C++. As such, Fit solves many problems relevant to C++ programmers, including initialization of function objects and lambdas, overloading with ordering, improved return type deduction, and much more.
- Lightweight: Fit builds simple lightweight abstraction on top of function objects. It does not require subscribing to an entire framework. Just use the parts you need.

Fit is divided into three components:

* Function Adaptors and Decorators: These enhance functions with additional capability.
* Functions: These return functions that achieve a specific purpose.
* Utilities: These are general utilities that are useful when defining or using functions

Github: [http://github.com/pfultz2/Fit](http://github.com/pfultz2/Fit)

Documentation: [http://fit.readthedocs.org](http://fit.readthedocs.org)

Motivation
==========

- Improve the expressiveness and capabilities of functions, including first-class citzens for function overload set, extension methods, infix operators and much more.
- Simplify constructs in C++ that have generally required metaprogramming
- Enable point-free style programming
- Workaround the limitations of lambdas in C++14

Requirements
============

This requires a C++11 compiler. There are no third-party dependencies. This has been tested on clang 3.5-3.8, gcc 4.6-6.2, and Visual Studio 2015. Gcc 5.1 is not supported at all, however, gcc 5.4 is supported.

Contexpr support
----------------

Both MSVC and gcc 4.6 have limited constexpr support due to many bugs in the implementation of constexpr. However, constexpr initialization of functions is supported when using the `FIT_STATIC_FUNCTION` and `FIT_STATIC_LAMBDA_FUNCTION` constructs.

