set(executables
  constructor
)

foreach(exec ${executables})
    # - Define executables ------
    add_executable(${exec} ${exec}.cpp)

    # - Link libraries ---------
    target_link_libraries(${exec} PRIVATE benchmark::benchmark_main)
    target_link_libraries(${exec} PRIVATE Igor)
endforeach()

