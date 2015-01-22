Overview
========

Fit is a header-only C++11 library that provides utilities for functions and function objects. 

Fit is:

- Modern: Fit takes advantages of modern C++11/C++14 features. It support both `constexpr` initialization and `constexpr` evaluation of functions. It takes advantage of type deduction, varidiac templates, and perfect forwarding to provide a simple and modern interface. 
- Relevant: Fit provides utilities for functions and does not try to implement a functional language in C++. As such, Fit solves many problems relevant to C++ programmers, including initialization of function objects and lambdas, overloading with ordering, improved return type deduction, and much more.
- Lightweight: Fit builds simple lightweight abstraction on top of function objects. It does not require subscribing to an entire framework. Just use the parts you need.

Fit is divided into three components:

* Function Adaptors: These take functions and return a new function that provides an additional capability to the previous function.
* Functions: These return functions that achieve a specific purpose.
* Utilities: These are general utilities that are useful when defining or using functions

