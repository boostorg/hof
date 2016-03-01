Building
========

The Fit library uses cmake to build. To configure with cmake create a build directory, and run cmake:

    mkdir build
    cd build
    cmake ..

Installing
----------

To install the library just run the `install` target:

    cmake --build . --target install

Tests
-----

The tests can be built and run by using the `check` target:

    cmake --build . --target check

Documentation
-------------

The documentation is built using mkdocs. Mkdocs can be easily installed by using `pip`:

    pip install mkdocs

Then the docs can be built using these commands:

    python setup.py
    mkdocs build

The final docs will be in the `doc/html` folder.

