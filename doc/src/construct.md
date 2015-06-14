construct
=========

Description
-----------

The `construct` function returns a function object that will construct the
object when the called. A template can also be givien, which it will
deduce the parameters to the template.

Synopsis
--------

    template<class T>
    constexpr auto construct();

    template<template<class...> class Template>
    constexpr auto construct();

Requirements
------------

T or Template must be:

    MoveConstructible

Example
-------

    auto v = fit::construct<std::vector<int>>()(5, 5);
    assert(v.size() == 5);

