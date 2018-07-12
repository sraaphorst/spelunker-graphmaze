# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

project(spelunker_graphmaze VERSION 0.1.0 LANGUAGES CXX)

set(PUBLIC_HEADER_FILES
        AldousBroderMazeGenerator.h
        BFSMazeGenerator.h
        GraphUtils.h
        MazeGraph.h
        MazeGenerator.h
        )

set(SOURCE_FILES
        AldousBroderMazeGenerator.cpp
        BFSMazeGenerator.cpp
        GraphUtils.cpp
        )

add_library(spelunker_graphmaze SHARED
        ${PUBLIC_HEADER_FILES}
        ${SOURCE_FILES}
        )

