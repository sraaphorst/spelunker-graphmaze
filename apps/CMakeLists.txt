# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

add_executable(dfs dfs.cpp)
target_link_libraries(dfs LINK_PUBLIC spelunker_graphmaze)

add_executable(aldous_broder aldous_broder.cpp)
target_link_libraries(aldous_broder LINK_PUBLIC spelunker_graphmaze)

add_executable(bfs bfs.cpp)
target_link_libraries(bfs LINK_PUBLIC spelunker_graphmaze)

add_executable(binary_tree binary_tree.cpp)
target_link_libraries(binary_tree LINK_PUBLIC spelunker_graphmaze)

add_executable(hunt_and_kill hunt_and_kill.cpp)
target_link_libraries(hunt_and_kill LINK_PUBLIC spelunker_graphmaze)

configure_file(mask.txt mask.txt COPYONLY)
configure_file(mask2.txt mask2.txt COPYONLY)
add_executable(grid_mask grid_mask.cpp)
target_link_libraries(grid_mask LINK_PUBLIC spelunker_graphmaze)

add_executable(sidewinder sidewinder.cpp)
target_link_libraries(sidewinder LINK_PUBLIC spelunker_graphmaze)