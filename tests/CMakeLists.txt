add_executable(test_coloring test_coloring.cpp)
target_link_libraries(test_coloring PRIVATE ctgraph)
add_test(NAME ColoringTest COMMAND test_coloring)
