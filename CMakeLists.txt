# CMakeLists.txt
# By Sebastian Raaphorst, 2018.
cmake_minimum_required(VERSION 3.10)
project(graphmaze)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Find includes in corresponding build directories.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Boost 1.67 COMPONENTS serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

message("Starting compilation of graphmaze...")
add_subdirectory(graphmaze)
message("Done")

message("Starting compilation of apps...")
add_executable(dfs dfs.cpp)
target_link_libraries(dfs spelunker_graphmaze)

add_executable(aldous_broder aldous_broder.cpp)
target_link_libraries(aldous_broder spelunker_graphmaze)

add_executable(bfs bfs.cpp)
target_link_libraries(bfs spelunker_graphmaze)

message("Done")