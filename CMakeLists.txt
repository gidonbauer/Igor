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

option(IGOR_BUILD_TESTS OFF)
if(IGOR_BUILD_TESTS)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(CMAKE_BUILD_TYPE "Debug")
  add_compile_options(-Wall -Wextra -pedantic -Wconversion -Wshadow)
  # add_compile_options(-fsanitize=address,undefined,leak -O0)
  # add_link_options(-fsanitize=address,undefined,leak)
  add_compile_options(-fsanitize=address,undefined -O0)
  add_link_options(-fsanitize=address,undefined)

  include(FetchContent)
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
