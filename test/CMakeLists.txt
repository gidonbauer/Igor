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

include(GoogleTest)
foreach(exec ${executables})
    # - Define executables ------
    add_executable(${exec} ${exec}.cpp)

    # - Link libraries ---------
    target_link_libraries(${exec} PRIVATE GTest::gtest_main GTest::gtest)
    target_link_libraries(${exec} PRIVATE Igor)

    # Export the symbols of the executable so that they show up in the stacktrace (`-rdynamic`).
    set_target_properties(${exec} PROPERTIES ENABLE_EXPORTS ON)

    gtest_discover_tests(${exec})
endforeach()
