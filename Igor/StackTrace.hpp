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
#include <version>

// - Maximum number of stack frames that are captured
// -----------------------------------------------
#ifndef IGOR_STACKTRACE_MAX_FRAMES
#define IGOR_STACKTRACE_MAX_FRAMES 64
#endif  // IGOR_STACKTRACE_MAX_FRAMES

// - Select the backend
// -----------------------------------------------------------------------------
// `IGOR_NO_STACKTRACE`:       Disable the stacktrace entirely, `Igor::stacktrace` returns an empty
//                             string and `Igor::print_stacktrace` prints nothing.
// `IGOR_USE_STD_STACKTRACE`:  Use `std::stacktrace` from C++23. This gives the best results
//                             (function name, file and line) but requires linking against
//                             `stdc++exp` when using libstdc++.
//                             TODO: Make this the default once `<stacktrace>` is widely available.
// default:                    Use `backtrace` from `<execinfo.h>` on POSIX systems, no additional
//                             linking required. Symbol names are demangled via `<cxxabi.h>` unless
//                             `IGOR_NO_CXX_ABI` is defined.
#if defined(IGOR_NO_STACKTRACE)

#define IGOR_STACKTRACE_BACKEND_NONE

#elif defined(IGOR_USE_STD_STACKTRACE)

#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
#define IGOR_STACKTRACE_BACKEND_STD
#include <stacktrace>
#else
#error                                                                                             \
    "`IGOR_USE_STD_STACKTRACE` was requested but this standard library does not provide `<stacktrace>`; remove the macro to fall back to `backtrace` or define `IGOR_NO_STACKTRACE` to disable stacktraces."
#endif  // __cpp_lib_stacktrace

#elif __has_include(<execinfo.h>)

#define IGOR_STACKTRACE_BACKEND_EXECINFO
#include <cstdlib>
#include <execinfo.h>
#include <memory>
#ifndef IGOR_NO_CXX_ABI
#include <cxxabi.h>
#endif  // IGOR_NO_CXX_ABI

#else

// TODO: Implement a backend for Windows using `CaptureStackBackTrace` from `<dbghelp.h>`; this
//       requires linking against `dbghelp` and is therefore not done here.
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

#ifdef IGOR_STACKTRACE_BACKEND_EXECINFO

[[nodiscard]] constexpr auto is_mangled_name_char(char c) noexcept -> bool {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' ||
         c == '$' || c == '.';
}

[[nodiscard]] inline auto demangle_line(std::string_view line) noexcept -> std::string {
  try {
#ifdef IGOR_NO_CXX_ABI
    return std::string{line};
#else
    constexpr auto free_deleter = [](void* p) constexpr noexcept {
      std::free(p);  // NOLINT(cppcoreguidelines-owning-memory,cppcoreguidelines-no-malloc)
    };

    // The module name might contain `_Z` as well, therefore we try all candidates.
    for (auto begin = line.find("_Z"); begin != std::string_view::npos;
         begin      = line.find("_Z", begin + 1)) {
      auto end = begin;
      while (end < line.size() && is_mangled_name_char(line[end])) {
        ++end;
      }
      const std::string mangled{line.substr(begin, end - begin)};

      int status = 0;
      const std::unique_ptr<char, decltype(free_deleter)> demangled{
          abi::__cxa_demangle(mangled.c_str(), nullptr, nullptr, &status), free_deleter};
      if (status != 0 || demangled == nullptr) { continue; }

      std::string res{line.substr(0, begin)};
      res += demangled.get();
      res += line.substr(end);
      return res;
    }
    return std::string{line};
#endif  // IGOR_NO_CXX_ABI
  } catch (...) { return std::string{line}; }
}

[[nodiscard]] constexpr auto strip_frame_number(std::string_view line) noexcept
    -> std::string_view {
  size_t idx = 0;
  while (idx < line.size() && line[idx] >= '0' && line[idx] <= '9') {
    ++idx;
  }
  if (idx == 0 || idx == line.size() || line[idx] != ' ') { return line; }
  while (idx < line.size() && line[idx] == ' ') {
    ++idx;
  }
  return line.substr(idx);
}

#endif  // IGOR_STACKTRACE_BACKEND_EXECINFO

}  // namespace detail

// -------------------------------------------------------------------------------------------------
[[nodiscard]] IGOR_STACKTRACE_NOINLINE inline auto stacktrace([[maybe_unused]] size_t skip = 0)
    -> std::string {
#if defined(IGOR_STACKTRACE_BACKEND_STD)

  try {
    const auto trace =
        std::stacktrace::current(skip + 1, static_cast<size_t>(IGOR_STACKTRACE_MAX_FRAMES));
    std::ostringstream out{};
    size_t frame_number = 0;
    for (const auto& entry : trace) {
      out << '#' << std::left << std::setw(3) << frame_number << ' ' << entry.description();
      if (!entry.source_file().empty()) {
        out << " at \033[95m" << entry.source_file() << ':' << entry.source_line() << "\033[0m";
      }
      out << '\n';
      ++frame_number;
    }
    return out.str();
  } catch (...) { return ""; }

#elif defined(IGOR_STACKTRACE_BACKEND_EXECINFO)

  try {
    constexpr auto free_deleter = [](void* p) constexpr noexcept {
      std::free(p);  // NOLINT(cppcoreguidelines-owning-memory,cppcoreguidelines-no-malloc)
    };

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
    void* frames[IGOR_STACKTRACE_MAX_FRAMES]{};
    const auto num_frames = ::backtrace(static_cast<void**>(frames), IGOR_STACKTRACE_MAX_FRAMES);
    if (num_frames <= 0) { return ""; }

    const std::unique_ptr<char*, decltype(free_deleter)> symbols{
        ::backtrace_symbols(static_cast<void**>(frames), num_frames), free_deleter};
    if (symbols == nullptr) { return ""; }

    std::ostringstream out{};
    size_t frame_number = 0;
    // `stacktrace` itself is frame `0`, therefore we skip one additional frame.
    for (size_t i = skip + 1; i < static_cast<size_t>(num_frames); ++i) {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      const std::string_view line{symbols.get()[i]};
      out << '#' << std::left << std::setw(3) << frame_number << ' '
          << detail::demangle_line(detail::strip_frame_number(line)) << '\n';
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
