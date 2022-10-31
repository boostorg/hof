<!-- Copyright 2018 Paul Fultz II
     Distributed under the Boost Software License, Version 1.0.
     (http://www.boost.org/LICENSE_1_0.txt)
-->

Building
========

Boost.HigherOrderFunctions library uses cmake to build. To configure with cmake create a build directory, and run cmake:

```cpp
mkdir build
cd build
cmake ..
```

Installing
----------

To install the library just run the `install` target:

```cpp
cmake --build . --target install
```

Tests
-----

The tests can be built and run by using the `check` target:

```cpp
cmake --build . --target check
```

The tests can also be ran using Boost.Build, just copy library to the boost source tree, and then:

```cpp
cd test
b2
```

Documentation
-------------

The documentation is built using Sphinx. First, install the requirements needed for the documentation using `pip`:

```cpp
pip install -r doc/requirements.txt
```

Then html documentation can be generated using `sphinx-build`:

```cpp
sphinx-build -b html doc/ doc/html/
```

The final docs will be in the `doc/html` folder.

