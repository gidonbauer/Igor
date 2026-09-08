set(executables
  test_MemoryToString
  test_StripPath
  test_Math
  test_Defer
  test_OnDeath
  test_Assert
  test_DisableAssert
  test_Logging
  test_StackTrace
  test_MdArray

  test_StaticVector_Initialize
  test_StaticVector_Destruct
  test_StaticVector_Iterator
  test_StaticVector_Insert
  test_StaticVector_Erase
)

# `Igor` never provides a stacktrace backend, but the tests need one to test the stacktrace.
# `cpptrace` has to be installed separately, e.g. via `brew install cpptrace`; if it is not found the
# stacktrace tests skip themselves.
find_package(cpptrace QUIET)
if(cpptrace_FOUND)
  message(STATUS "Found cpptrace, testing the stacktrace with `IGOR_USE_CPPTRACE`")
else()
  message(STATUS "Did not find cpptrace, the stacktrace tests are skipped")
endif()

include(GoogleTest)
foreach(exec ${executables})
    # - Define executables ------
    add_executable(${exec} ${exec}.cpp)

    # - Link libraries ---------
    target_link_libraries(${exec} PRIVATE GTest::gtest_main GTest::gtest)
    target_link_libraries(${exec} PRIVATE Igor)
    if(cpptrace_FOUND)
      target_link_libraries(${exec} PRIVATE cpptrace::cpptrace)
      target_compile_definitions(${exec} PRIVATE IGOR_USE_CPPTRACE)
    endif()

    gtest_discover_tests(${exec})
endforeach()
