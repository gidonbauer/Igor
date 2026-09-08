// Copyright 2024 Gidon Bauer <gidon.bauer@rwth-aachen.de>

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef IGOR_STACK_TRACE_HPP_
#define IGOR_STACK_TRACE_HPP_

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

// - Maximum number of stack frames that are captured ----------------------------------------------
#ifndef IGOR_STACKTRACE_MAX_FRAMES
#define IGOR_STACKTRACE_MAX_FRAMES 64
#endif  // IGOR_STACKTRACE_MAX_FRAMES

// - Select the backend ----------------------------------------------------------------------------
// `IGOR_USE_STD_STACKTRACE`: Use `std::stacktrace` from C++23
// `IGOR_USE_CPPTRACE`:       Use `cpptrace` (https://github.com/jeremy-rifkin/cpptrace).
//                            Requires including the library yourself.
#if defined(IGOR_USE_STD_STACKTRACE) && defined(IGOR_USE_CPPTRACE)
#error "`IGOR_USE_STD_STACKTRACE` and `IGOR_USE_CPPTRACE` are mutually exclusive, define only one."
#endif  // IGOR_USE_STD_STACKTRACE && IGOR_USE_CPPTRACE

#if defined(IGOR_USE_STD_STACKTRACE)

#include <version>
#if !defined(__cpp_lib_stacktrace) || __cpp_lib_stacktrace < 202011L
#error                                                                                             \
    "`IGOR_USE_STD_STACKTRACE` was requested but this standard library does not provide `<stacktrace>`; use `IGOR_USE_CPPTRACE` instead or define neither to disable stacktraces."
#endif  // __cpp_lib_stacktrace
#define IGOR_STACKTRACE_BACKEND_STD
#include <stacktrace>

#elif defined(IGOR_USE_CPPTRACE)

#if !__has_include(<cpptrace/cpptrace.hpp>)
#error                                                                                             \
    "`IGOR_USE_CPPTRACE` was requested but `<cpptrace/cpptrace.hpp>` was not found; install `cpptrace` (e.g. `brew install cpptrace`), add its include directory and link against it."
#endif  // __has_include(<cpptrace/cpptrace.hpp>)
#define IGOR_STACKTRACE_BACKEND_CPPTRACE
#include <cpptrace/cpptrace.hpp>

#else

#define IGOR_STACKTRACE_BACKEND_NONE

#endif  // Select the backend

#if defined(_MSC_VER)
#define IGOR_STACKTRACE_NOINLINE __declspec(noinline)
#else
#define IGOR_STACKTRACE_NOINLINE __attribute__((noinline))
#endif  // IGOR_STACKTRACE_NOINLINE

namespace Igor {

// -------------------------------------------------------------------------------------------------
inline constexpr bool stacktrace_available =
#ifdef IGOR_STACKTRACE_BACKEND_NONE
    false;
#else
    true;
#endif  // IGOR_STACKTRACE_BACKEND_NONE

namespace detail {

#ifndef IGOR_STACKTRACE_BACKEND_NONE

// Format a single frame as `#<frame_number> <symbol> at <file>:<line>`; the location is omitted if
// the debug information is not available.
inline void format_frame(std::ostream& out,
                         size_t frame_number,
                         std::string_view symbol,
                         std::string_view file,
                         size_t line,
                         bool is_inline = false) noexcept {
  out << '#' << std::left << std::setw(3) << frame_number << ' '
      << (symbol.empty() ? "<unknown>" : symbol);
  if (is_inline) { out << " [inlined]"; }
  if (!file.empty()) { out << " at \033[95m" << file << ':' << line << "\033[0m"; }
  out << '\n';
}

#endif  // IGOR_STACKTRACE_BACKEND_NONE

}  // namespace detail

// -------------------------------------------------------------------------------------------------
[[nodiscard]] IGOR_STACKTRACE_NOINLINE inline auto stacktrace([[maybe_unused]] size_t skip = 0)
    -> std::string {
#if defined(IGOR_STACKTRACE_BACKEND_STD)

  try {
    // `stacktrace` itself is frame `0`, therefore we skip one additional frame.
    const auto trace =
        std::stacktrace::current(skip + 1, static_cast<size_t>(IGOR_STACKTRACE_MAX_FRAMES));
    std::ostringstream out{};
    size_t frame_number = 0;
    for (const auto& frame : trace) {
      detail::format_frame(
          out, frame_number, frame.description(), frame.source_file(), frame.source_line());
      ++frame_number;
    }
    return out.str();
  } catch (...) { return ""; }

#elif defined(IGOR_STACKTRACE_BACKEND_CPPTRACE)

  try {
    // `stacktrace` itself is frame `0`, therefore we skip one additional frame.
    const auto trace =
        cpptrace::stacktrace::current(skip + 1, static_cast<size_t>(IGOR_STACKTRACE_MAX_FRAMES));
    std::ostringstream out{};
    size_t frame_number = 0;
    for (const auto& frame : trace) {
      detail::format_frame(out,
                           frame_number,
                           frame.symbol,
                           frame.line.has_value() ? frame.filename : "",
                           frame.line.value_or(0),
                           frame.is_inline);
      ++frame_number;
    }
    return out.str();
  } catch (...) { return ""; }

#else

  return "";

#endif  // IGOR_STACKTRACE_BACKEND_*
}

// -------------------------------------------------------------------------------------------------
IGOR_STACKTRACE_NOINLINE inline void print_stacktrace(std::ostream& out = std::cerr,
                                                      size_t skip       = 0) noexcept {
  if constexpr (stacktrace_available) {
    try {
      const auto trace = stacktrace(skip + 1);
      if (trace.empty()) { return; }
      out << "\033[36m[STACKTRACE]\033[0m\n" << trace << std::flush;
    } catch (...) {  // NOLINT(bugprone-empty-catch)
      // Printing the stacktrace is best effort only, never fail because of it.
    }
  }
}

}  // namespace Igor

#endif  // IGOR_STACK_TRACE_HPP_
