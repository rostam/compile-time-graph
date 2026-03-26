add_executable(test_coloring test_coloring.cpp)
target_link_libraries(test_coloring PRIVATE ctgraph)
add_test(NAME ColoringTest COMMAND test_coloring)

add_executable(test_graph test_graph.cpp)
target_link_libraries(test_graph PRIVATE ctgraph)
add_test(NAME GraphTest COMMAND test_graph)

add_executable(test_traversal test_traversal.cpp)
target_link_libraries(test_traversal PRIVATE ctgraph)
add_test(NAME TraversalTest COMMAND test_traversal)

add_executable(test_generators test_generators.cpp)
target_link_libraries(test_generators PRIVATE ctgraph)
add_test(NAME GeneratorsTest COMMAND test_generators)

add_executable(test_properties test_properties.cpp)
target_link_libraries(test_properties PRIVATE ctgraph)
add_test(NAME PropertiesTest COMMAND test_properties)
