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
                ./Igor/TypeName.hpp)
target_include_directories(Igor INTERFACE .)
