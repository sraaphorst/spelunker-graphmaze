# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

message("Starting compilation of graphmaze...")
message("Done")

message("Starting compilation of apps...")
add_executable(dfs dfs.cpp)
target_link_libraries(dfs LINK_PUBLIC spelunker_graphmaze)

add_executable(aldous_broder aldous_broder.cpp)
target_link_libraries(aldous_broder LINK_PUBLIC spelunker_graphmaze)

add_executable(bfs bfs.cpp)
target_link_libraries(bfs LINK_PUBLIC spelunker_graphmaze)

add_executable(hunt_and_kill hunt_and_kill.cpp)
target_link_libraries(hunt_and_kill LINK_PUBLIC spelunker_graphmaze)

message("Done")