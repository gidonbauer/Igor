# Igor - A simple utility library in C++.

`Igor` is a simple header only library written in C++20 that adds some utility functions.

## Capabilities

- Simple logging to `stdout` and `stderr`
    - Include source location for warnings and errors
    - Build upon C++20 format
- De-mangling C++ type names to a string
- Simple timing of scopes
- Simple command line progressbar, non-thread safe

## Add to your project

Simply copy `Igor.hpp` into your project and include it in your C++ files.
`Igor.hpp` depends only on the C++ standard library and on `cxxabi.h`.
The later one can be disabled via the macro `IGOR_NO_CXX_ABI`.