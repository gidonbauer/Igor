cmake_minimum_required(VERSION 3.20)

project(Igor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include(FetchContent)
# FetchContent_Declare(
#   fmt
#   GIT_REPOSITORY https://github.com/fmtlib/fmt
#   GIT_TAG        11.0.2
# )
# FetchContent_MakeAvailable(fmt)
# add_compile_definitions(IGOR_USE_FMT)

add_library(Igor INTERFACE
                ./Igor/Defer.hpp
                ./Igor/Igor.hpp
                ./Igor/Logging.hpp
                ./Igor/Macros.hpp
                ./Igor/Math.hpp
                ./Igor/MemoryToString.hpp
                ./Igor/ProgressBar.hpp
                ./Igor/StaticVector.hpp
                ./Igor/Timer.hpp
                ./Igor/TypeName.hpp
                ./Igor/MdArray.hpp
                ./Igor/MdspanToNpy.hpp)
target_include_directories(Igor INTERFACE .)

include(FetchContent)
option(IGOR_BUILD_TESTS OFF)
option(IGOR_BUILD_BENCHMARKS OFF)

if(IGOR_BUILD_TESTS)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(CMAKE_BUILD_TYPE "Debug")
  add_compile_options(-Wall -Wextra -pedantic -Wconversion -Wshadow)
  # add_compile_options(-fsanitize=address,undefined,leak -O0)
  # add_link_options(-fsanitize=address,undefined,leak)
  add_compile_options(-fsanitize=address,undefined -O0)
  add_link_options(-fsanitize=address,undefined)

  enable_testing()

  set(BUILD_GMOCK OFF)
  FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG        main
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  message(STATUS "Build tests")
  
  add_subdirectory(${CMAKE_SOURCE_DIR}/test/)
endif()

if(IGOR_BUILD_BENCHMARKS)
  set(CMAKE_BUILD_TYPE "Release")
  add_compile_options(-Wall -Wextra -pedantic -Wconversion -Wshadow)
  # add_compile_options(-fsanitize=address,undefined,leak -O0)
  # add_link_options(-fsanitize=address,undefined,leak)
  add_compile_options(-O3 -march=native)

  set(BENCHMARK_ENABLE_TESTING OFF)
  set(BENCHMARK_ENABLE_INSTALL OFF)
  FetchContent_Declare(
      benchmark
      GIT_REPOSITORY https://github.com/google/benchmark.git
      GIT_TAG        main
  )
  FetchContent_MakeAvailable(benchmark)

  message(STATUS "Build benchmarks")
  
  add_subdirectory(${CMAKE_SOURCE_DIR}/bench/)
endif()
