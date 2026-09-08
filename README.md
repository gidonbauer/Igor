# Igor - A simple utility library in C++.

`Igor` is a simple header only library written in C++20 that adds some utility functions.

## Capabilities

- `Igor/Logging.hpp`: Simple logging to `stdout` and `stderr`
    - Include source location for warnings and errors
    - Build upon C++20 format
    - Print the stacktrace when an assertion fails or when `Igor::Panic` is called
- `Igor/StackTrace.hpp`: Capture and print the stacktrace of the calling thread
    - `IGOR_NO_STACKTRACE`: Disable stacktrace
    - `IGOR_USE_STD_STACKTRACE`: Use `std::stacktrace` from C++23
    - default: Use `backtrace` from `<execinfo.h>` on POSIX systems
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
