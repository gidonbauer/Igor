# Igor - A simple utility library in C++.

`Igor` is a simple header only library written in C++20 that adds some utility functions.

## Capabilities

- `Igor/Logging.hpp`: Simple logging to `stdout` and `stderr`
    - Include source location for warnings and errors
    - Build upon C++20 format
    - Print the stacktrace when an assertion fails or when `Igor::Panic` is called
- `Igor/StackTrace.hpp`: Capture and print the stacktrace of the calling thread, both backends are
  opt-in and mutually exclusive
    - default: No stacktrace, `Igor` stays header only and free of dependencies
    - `IGOR_USE_CPPTRACE`: Use [cpptrace](https://github.com/jeremy-rifkin/cpptrace)
    - `IGOR_USE_STD_STACKTRACE`: Use `std::stacktrace` from C++23
    - `IGOR_STACKTRACE_MAX_FRAMES`: Maximum number of captured stack frames
- `Igor/TypeName.hpp`: De-mangling C++ type names to a string
- `Igor/Timer.hpp`: Simple timing of scopes
- `Igor/ProgressBar.hpp`: Simple command line progressbar, non-thread safe
- `Igor/Macros.hpp`: Some useful preprocessor macros
- `Igor/StaticVector.hpp`: Static stack vector, implements the std::vector interface

## Add to your project

Simply copy `Igor/` into your project and include the necessary headers in your C++ files.
`Igor` depends only on the C++ standard library and on `cxxabi.h`.
The later one can be disabled via the macro `IGOR_NO_CXX_ABI`.

Printing a stacktrace requires a backend.
If you chose [cpptrace](https://github.com/jeremy-rifkin/cpptrace), you must add the include path and link with the library yourself, for example
```sh
clang++ -std=c++23 -O3 -g -I/path/to/Igor \
        -DIGOR_USE_CPPTRACE -I/path/to/cpptrace/include -L/path/to/cpptrace/lib -lcpptrace \
        -o my_program my_program.cpp
```
