Configurations
==============

There are several configuration that control the behaviour in the Fit library.

+-------------------------------+--------------------------------------------------------------------------------+
| Name                          | Description                                                                    |
+===============================+================================================================================+
| FIT_CHECK_UNPACK_SEQUENCE     | Unpack has extra checks to ensure that the function will be invoked with the   |
|                               | sequence. This extra check can help improve error reporting but it can slow    |
|                               | down compilation. This is enabled by default.                                  |
+-------------------------------+--------------------------------------------------------------------------------+
| FIT_NO_EXPRESSION_SFINAE      | This controls whether the Fit library will use expression SFINAE to detect the |
|                               | callability of functions. On MSVC, this is enabled by default, since it does   |
|                               | not have full support for expression SFINAE.                                   |
+-------------------------------+--------------------------------------------------------------------------------+
|                               |                                                                                |
| FIT_RECURSIVE_CONSTEXPR_DEPTH | Because C++ instantiates `constexpr` functions eagerly, recursion with         |
|                               | `constexpr` functions can cause the compiler to reach its internal limits. The |
|                               | setting is used by Fit to set a limit on recursion depth when using            |
|                               | `constexpr`. The default is 16, but increasing the limit can increase compile  |
|                               | times.                                                                         |
+-------------------------------+--------------------------------------------------------------------------------+



